// SpriteTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "SpriteTool.h"
#include "afxdialogex.h"


// CSpriteTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSpriteTool, CDialog)

CSpriteTool::CSpriteTool(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SPRITETOOL, pParent), m_iCenterX(0), m_iCenterY(0), m_pDeviceMgr(DEVICEMGR), m_pTextureMgr(TEXTUREMGR), m_iTextureWidth(0), m_iTextureHeight(0), m_aniIndex(0){

}

CSpriteTool::~CSpriteTool(){
	for(auto& elem : m_InfoList)
		SafeDelete(elem);
}

void CSpriteTool::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeCtrl);
	DDX_Text(pDX, IDC_EDIT1, m_iCenterX);
	DDX_Text(pDX, IDC_EDIT2, m_iCenterY);
	DDX_Text(pDX, IDC_STATIC_WIDTH, m_iTextureWidth);
	DDX_Text(pDX, IDC_STATIC_HEIGHT, m_iTextureHeight);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_ColorCtrl);
	DDX_Control(pDX, IDC_STATIC_SPRITEPREVIEW, m_SpritePriview);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl);
}


BEGIN_MESSAGE_MAP(CSpriteTool, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CSpriteTool::OnTvnSelchangedTree1)
	ON_BN_CLICKED(IDC_BUTTON1, &CSpriteTool::OnBnClickedApply)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, &CSpriteTool::OnNMReleasedcaptureSlider1)
	ON_BN_CLICKED(IDC_BUTTON2, &CSpriteTool::OnBnClickedPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON6, &CSpriteTool::OnBnClickedSaveSetting)
	ON_BN_CLICKED(IDC_BUTTON5, &CSpriteTool::OnBnClickedStop)
END_MESSAGE_MAP()


// CSpriteTool 메시지 처리기입니다.

//스프라이트 다이얼로그 생성시 발생하는 이벤트 처리기
BOOL CSpriteTool::OnInitDialog(){
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	wifstream fin;

	//텍스쳐 로드 정보를 기록한 텍스트 파일 경로
	fin.open(L"../Data/TextruePath.txt");

	if(fin.fail()){
		::AfxMessageBox(L"SpriteTool Tree Load Fail");
		return TRUE;
	}

	//트리 컨트롤에 입력할 문자열
	TCHAR TreeTypeKey[MAX_STR] = L"";
	TCHAR TreeObjectKey[MAX_STR] = L"";
	TCHAR TreeStateKey[MAX_STR] = L"";
	TCHAR TreeDirectionKey[MAX_STR] = L"";

	//읽은 파일의 첫줄은 무시함
	fin.getline(TreeTypeKey, MAX_STR);
	lstrcmp(TreeTypeKey, L"");

	//트리에 아이템 삽입 후 결과를 받을 변수들
	HTREEITEM itemType = nullptr, itemObject = nullptr, itemState = nullptr, itemCurr = nullptr;	//itemCurr는 아이템과 데이터를 연결하기 위해 말단 노드를 지정할 핸들임

	while(true){
		//각 정보를 받을 문자열들
		TCHAR wstrTypeKey[MAX_STR] = L"";
		TCHAR wstrObjectKey[MAX_STR] = L"";
		TCHAR iImgCount[MAX_STR] = L"";
		TCHAR wstrStateKey[MAX_STR] = L"";
		TCHAR wstrDirectionKey[MAX_STR] = L"";
		TCHAR iColorA[MAX_STR] = L"";
		TCHAR iColorR[MAX_STR] = L"";
		TCHAR iColorG[MAX_STR] = L"";
		TCHAR iColorB[MAX_STR] = L"";
		TCHAR iCenterX[MAX_STR] = L"";
		TCHAR iCenterY[MAX_STR] = L"";
		TCHAR wstrImgPath[MAX_STR] = L"";

		//////////////////////////////////////////////////////////////////////
		//타입키
		fin.getline(wstrTypeKey, MAX_STR, '|');
		//받아온 타입키가 없다면 while문 종료(타입키가 없으면 더 이상 읽을 정보가 없음)
		if(0 == lstrlen(wstrTypeKey))
			break;
		//이전에 저장된 트리의 타입키와 다르다면 새로 트리에 아이템 삽입(중복된 타입키가 아니라면 목록을 새로 만든다)
		if(0 != lstrcmp(wstrTypeKey, TreeTypeKey)){
			//아이템 삽입 후에 반환되는 아이템들은 트리의 부모로 사용되기 때문에 따로 보관
			itemCurr = itemType = m_TreeCtrl.InsertItem(wstrTypeKey);
			lstrcpy(TreeTypeKey, wstrTypeKey);
		}
		//////////////////////////////////////////////////////////////////////
		//오브젝트키
		fin.getline(wstrObjectKey, MAX_STR, '|');
		//이전에 저장된 트리의 오브젝트키와 다르다면 새로 트리에 아이템 삽입(중복된 타입키가 아니라면 목록을 새로 만든다)
		if(0 != lstrcmp(wstrObjectKey, TreeObjectKey)){
			//부모 인자로 위에서 보관된 부모의 아이템 핸들을 사용
			itemCurr = itemObject = m_TreeCtrl.InsertItem(wstrObjectKey, itemType);
			lstrcpy(TreeObjectKey, wstrObjectKey);
		}
		//////////////////////////////////////////////////////////////////////

		//이미지 수
		fin.getline(iImgCount, MAX_STR, '|');

		//////////////////////////////////////////////////////////////////////
		//스테이트키
		fin.getline(wstrStateKey, MAX_STR, '|');
		//스테이트키가 없다면 트리 연결 중단
		if(0 == lstrlen(wstrStateKey)){
			lstrcpy(TreeStateKey, wstrStateKey);
		} else if(0 != lstrcmp(wstrStateKey, TreeStateKey)){
			itemCurr = itemState = m_TreeCtrl.InsertItem(wstrStateKey, itemObject);
			lstrcpy(TreeStateKey, wstrStateKey);
		}
		//////////////////////////////////////////////////////////////////////
		//디렉션키
		fin.getline(wstrDirectionKey, MAX_STR, '|');
		if(0 != lstrlen(TreeStateKey)){
			if(0 != lstrcmp(wstrDirectionKey, TreeDirectionKey)){
				itemCurr = m_TreeCtrl.InsertItem(wstrDirectionKey, itemState);
			}
		}
		//////////////////////////////////////////////////////////////////////

		//색상값
		fin.getline(iColorA, MAX_STR, '|');
		fin.getline(iColorR, MAX_STR, '|');
		fin.getline(iColorG, MAX_STR, '|');
		fin.getline(iColorB, MAX_STR, '|');

		//텍스쳐 중심값
		fin.getline(iCenterX, MAX_STR, '|');
		fin.getline(iCenterY, MAX_STR, '|');

		//텍스쳐 파일 경로
		fin.getline(wstrImgPath, MAX_STR);

		/*if(fin.eof())
			break;*/

		//읽어온 값들은 따로 구조체에 보관하여 관리
		PATH_INFO* pPathInfo = new PATH_INFO;
		pPathInfo->wstrTypeKey = wstrTypeKey;
		pPathInfo->wstrObjectKey = wstrObjectKey;
		pPathInfo->iImgCount = _ttoi(iImgCount);
		pPathInfo->wstrStateKey = wstrStateKey;
		pPathInfo->wstrDirectionKey = wstrDirectionKey;
		pPathInfo->iColorA = _ttoi(iColorA);
		pPathInfo->iColorR = _ttoi(iColorR);
		pPathInfo->iColorG = _ttoi(iColorG);
		pPathInfo->iColorB = _ttoi(iColorB);
		pPathInfo->iCenterX = _ttoi(iCenterX);
		pPathInfo->iCenterY = _ttoi(iCenterY);
		pPathInfo->wstrImgPath = wstrImgPath;

		//트리 컨트롤의 아이템에 데이터 연결(DWORD_PTR로 형변환 하여 연결)
		m_TreeCtrl.SetItemData(itemCurr, reinterpret_cast<DWORD_PTR>(pPathInfo));

		//관리하기 편하게 리스트에 따로 또 저장, 사실 이유없는 저장
		m_InfoList.push_back(pPathInfo);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//트리 컨트롤의 항목 선택시 발생하는 이벤트 처리기
void CSpriteTool::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	//타이머 종료
	KillTimer(1);

	//선택된 트리 컨트롤의 항목에 해당하는 데이터를 가져옴(DWORD_PTR형으로 변환되어 있기 때문에 다시 PATH_INFO*로 형변환함)
	PATH_INFO* pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

	//가져온 데이터 없으면 return
	if(nullptr == pInfo)
		return;

	//////////////////////////////////////////////////////////////////////
	//가져온 데이터를 각 컨트롤에 입력

	//중심점
	m_iCenterX = pInfo->iCenterX;
	m_iCenterY = pInfo->iCenterY;

	//색상값
	m_ColorCtrl.SetColor(RGB(pInfo->iColorR, pInfo->iColorG, pInfo->iColorB));

	//슬라이더 범위
	m_SliderCtrl.SetRange(0, pInfo->iImgCount - 1);

	//////////////////////////////////////////////////////////////////////

	//미리보기

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(pInfo->wstrTypeKey, pInfo->wstrObjectKey, 0, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	Render(pTextureInfo);

	//////////////////////////////////////////////////////////////////////

	UpdateData(FALSE);

	*pResult = 0;
}

//적용 버튼 클릭시 발생하는 이벤트 처리기
void CSpriteTool::OnBnClickedApply(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	//////////////////////////////////////////////////////////////////////

	//타이머 종료
	KillTimer(1);

	//선택된 트리 컨트롤의 항목에 해당하는 데이터를 가져옴(DWORD_PTR형으로 변환되어 있기 때문에 다시 PATH_INFO*로 형변환함)
	PATH_INFO* pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

	//가져온 데이터 없으면 return
	if(nullptr == pInfo)
		return;

	COLORREF tempColor = m_ColorCtrl.GetColor();

	//투명처리할 색상 RGB값 따로 추출(반대로 BGR로 되어있음)
	int iR = (tempColor & 0x000000FF);
	int iG = (tempColor & 0x0000FF00) >> 8;
	int iB = (tempColor & 0x00FF0000) >> 16;

	D3DCOLOR transColor = D3DCOLOR_ARGB(255, iR, iG, iB);

	//중심점을 D3DXVECTOR3로 변환
	D3DXVECTOR3 vec3Center = { static_cast<float>(m_iCenterX), static_cast<float>(m_iCenterY), 0.f };

	//중심점 설정
	m_pTextureMgr->SetTextureCenter(vec3Center, pInfo->wstrTypeKey, pInfo->wstrObjectKey, pInfo->wstrStateKey, pInfo->wstrDirectionKey);
	//투명색 설정
	m_pTextureMgr->SetTextureTrans(transColor, pInfo->wstrTypeKey, pInfo->wstrObjectKey, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	//각 정보를 따로 저장
	pInfo->iCenterX = m_iCenterX;
	pInfo->iCenterY = m_iCenterY;
	pInfo->iColorA = (transColor & 0xFF000000) >> 24;
	pInfo->iColorR = iR;
	pInfo->iColorG = iG;
	pInfo->iColorB = iB;

	//////////////////////////////////////////////////////////////////////

	//미리보기

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(pInfo->wstrTypeKey, pInfo->wstrObjectKey, 0, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	Render(pTextureInfo);

	UpdateData(FALSE);
}

//슬라이드를 클릭했을때 발생하는 이벤트 처리기
void CSpriteTool::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	//선택된 트리 컨트롤의 항목에 해당하는 데이터를 가져옴(DWORD_PTR형으로 변환되어 있기 때문에 다시 PATH_INFO*로 형변환함)
	PATH_INFO* pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

	//가져온 데이터 없으면 return
	if(nullptr == pInfo)
		return;

	int index = m_SliderCtrl.GetPos();

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(pInfo->wstrTypeKey, pInfo->wstrObjectKey, index, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	Render(pTextureInfo);

	*pResult = 0;
}

//재생 버튼 클릭시 발생하는 이벤트 처리기
void CSpriteTool::OnBnClickedPlay(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	SetTimer(1, 33, NULL);
}

//중지 버튼 클릭시 발생하는 이벤트 처리기
void CSpriteTool::OnBnClickedStop(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(1);
}

//타이머 이벤트 처리기
void CSpriteTool::OnTimer(UINT_PTR nIDEvent){
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	const TEXTURE_INFO* pTextureInfo = nullptr;
	PATH_INFO* pInfo = nullptr;

	switch(nIDEvent){
	case 1:
		pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

		if(nullptr == pInfo)
			break;

		// 출력할 인덱스 값 증가
		m_aniIndex = ++m_aniIndex % pInfo->iImgCount;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(pInfo->wstrTypeKey, pInfo->wstrObjectKey, m_aniIndex, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

		Render(pTextureInfo);

		m_SliderCtrl.SetPos(m_aniIndex);
		break;
	default:
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

//설정 버튼 클릭시 발생하는 이벤트 처리기
void CSpriteTool::OnBnClickedSaveSetting(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	wofstream fout;


	fout.open(L"../Data/TextruePath.txt");

	if(fout.fail())
		return;

	wstring wstrString = L"Type|Object|Image_Count||StateKey|DirectionKey|A|R|G|B|CenterX|CenterXY|FilePath";

	fout << wstrString.c_str() << endl;

	for(PATH_INFO* elem : m_InfoList){
		wstrString = elem->wstrTypeKey + L"|";
		wstrString += elem->wstrObjectKey + L"|";
		TCHAR itoString[MIN_STR] = {};

		swprintf_s(itoString, L"%d|", elem->iImgCount);
		wstrString += itoString;

		wstrString += elem->wstrStateKey + L"|";
		wstrString += elem->wstrDirectionKey + L"|";

		swprintf_s(itoString, L"%d|", elem->iColorA);
		wstrString += itoString;
		swprintf_s(itoString, L"%d|", elem->iColorR);
		wstrString += itoString;
		swprintf_s(itoString, L"%d|", elem->iColorG);
		wstrString += itoString;
		swprintf_s(itoString, L"%d|", elem->iColorB);
		wstrString += itoString;

		swprintf_s(itoString, L"%d|", elem->iCenterX);
		wstrString += itoString;
		swprintf_s(itoString, L"%d|", elem->iCenterY);
		wstrString += itoString;

		wstrString += elem->wstrImgPath;

		fout << wstrString.c_str() << endl;
	}

	fout.close();

}

//미리보기 렌더 함수
void CSpriteTool::Render(const TEXTURE_INFO * pTextureInfo){
	if(nullptr == pTextureInfo)
		return;

	//출력 텍스쳐의 크기 가져옴
	m_iTextureHeight = pTextureInfo->tImgInfo.Height;
	m_iTextureWidth = pTextureInfo->tImgInfo.Width;

	D3DXMATRIX matScale, matTrans, matWorld;
	//미리보기 창에 꽉 채워서 표시하기 위한 크기 조절
	D3DXMatrixScaling(&matScale, static_cast<float>(WINCX) / pTextureInfo->tImgInfo.Width, static_cast<float>(WINCY) / pTextureInfo->tImgInfo.Height, 0.f);
	//미리보기 창의 가운데로 이동
	D3DXMatrixTranslation(&matTrans, WINCX*0.5f, WINCY*0.5f, 0.f);

	//월드 행렬 = 크기 행렬 * 자전 행렬 * 이동 행렬 * 공전 행렬 * 부모 행렬 
	matWorld = matScale* matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	//렌더 준비
	m_pDeviceMgr->Render_Begin();
	//후면버퍼에 Draw
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture,
		nullptr,
		&pTextureInfo->vec3CenterPoint,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pDeviceMgr->GetSprite()->End();

	//중심점을 표시할 십자가
	D3DXVECTOR2 verCross[2] = { { WINCX*0.5f ,0.f },{ WINCX*0.5f,static_cast<float>(WINCY) } };
	D3DXVECTOR2 HoriCross[2] = { { 0.f, WINCY * 0.5f },{ static_cast<float>(WINCX), WINCY * 0.5f } };

	m_pDeviceMgr->GetLine()->SetWidth(3.f);
	m_pDeviceMgr->GetLine()->Begin();
	m_pDeviceMgr->GetLine()->Draw(verCross, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pDeviceMgr->GetLine()->Draw(HoriCross, 2, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pDeviceMgr->GetLine()->End();

	m_pDeviceMgr->Render_End(m_SpritePriview.m_hWnd);

	//////////////////////////////////////////////////////////////////////
}

