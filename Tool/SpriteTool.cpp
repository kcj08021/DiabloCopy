// SpriteTool.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "SpriteTool.h"
#include "afxdialogex.h"


// CSpriteTool ��ȭ �����Դϴ�.

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


// CSpriteTool �޽��� ó�����Դϴ�.

//��������Ʈ ���̾�α� ������ �߻��ϴ� �̺�Ʈ ó����
BOOL CSpriteTool::OnInitDialog(){
	CDialog::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	wifstream fin;

	//�ؽ��� �ε� ������ ����� �ؽ�Ʈ ���� ���
	fin.open(L"../Data/TextruePath.txt");

	if(fin.fail()){
		::AfxMessageBox(L"SpriteTool Tree Load Fail");
		return TRUE;
	}

	//Ʈ�� ��Ʈ�ѿ� �Է��� ���ڿ�
	TCHAR TreeTypeKey[MAX_STR] = L"";
	TCHAR TreeObjectKey[MAX_STR] = L"";
	TCHAR TreeStateKey[MAX_STR] = L"";
	TCHAR TreeDirectionKey[MAX_STR] = L"";

	//���� ������ ù���� ������
	fin.getline(TreeTypeKey, MAX_STR);
	lstrcmp(TreeTypeKey, L"");

	//Ʈ���� ������ ���� �� ����� ���� ������
	HTREEITEM itemType = nullptr, itemObject = nullptr, itemState = nullptr, itemCurr = nullptr;	//itemCurr�� �����۰� �����͸� �����ϱ� ���� ���� ��带 ������ �ڵ���

	while(true){
		//�� ������ ���� ���ڿ���
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
		//Ÿ��Ű
		fin.getline(wstrTypeKey, MAX_STR, '|');
		//�޾ƿ� Ÿ��Ű�� ���ٸ� while�� ����(Ÿ��Ű�� ������ �� �̻� ���� ������ ����)
		if(0 == lstrlen(wstrTypeKey))
			break;
		//������ ����� Ʈ���� Ÿ��Ű�� �ٸ��ٸ� ���� Ʈ���� ������ ����(�ߺ��� Ÿ��Ű�� �ƴ϶�� ����� ���� �����)
		if(0 != lstrcmp(wstrTypeKey, TreeTypeKey)){
			//������ ���� �Ŀ� ��ȯ�Ǵ� �����۵��� Ʈ���� �θ�� ���Ǳ� ������ ���� ����
			itemCurr = itemType = m_TreeCtrl.InsertItem(wstrTypeKey);
			lstrcpy(TreeTypeKey, wstrTypeKey);
		}
		//////////////////////////////////////////////////////////////////////
		//������ƮŰ
		fin.getline(wstrObjectKey, MAX_STR, '|');
		//������ ����� Ʈ���� ������ƮŰ�� �ٸ��ٸ� ���� Ʈ���� ������ ����(�ߺ��� Ÿ��Ű�� �ƴ϶�� ����� ���� �����)
		if(0 != lstrcmp(wstrObjectKey, TreeObjectKey)){
			//�θ� ���ڷ� ������ ������ �θ��� ������ �ڵ��� ���
			itemCurr = itemObject = m_TreeCtrl.InsertItem(wstrObjectKey, itemType);
			lstrcpy(TreeObjectKey, wstrObjectKey);
		}
		//////////////////////////////////////////////////////////////////////

		//�̹��� ��
		fin.getline(iImgCount, MAX_STR, '|');

		//////////////////////////////////////////////////////////////////////
		//������ƮŰ
		fin.getline(wstrStateKey, MAX_STR, '|');
		//������ƮŰ�� ���ٸ� Ʈ�� ���� �ߴ�
		if(0 == lstrlen(wstrStateKey)){
			lstrcpy(TreeStateKey, wstrStateKey);
		} else if(0 != lstrcmp(wstrStateKey, TreeStateKey)){
			itemCurr = itemState = m_TreeCtrl.InsertItem(wstrStateKey, itemObject);
			lstrcpy(TreeStateKey, wstrStateKey);
		}
		//////////////////////////////////////////////////////////////////////
		//�𷺼�Ű
		fin.getline(wstrDirectionKey, MAX_STR, '|');
		if(0 != lstrlen(TreeStateKey)){
			if(0 != lstrcmp(wstrDirectionKey, TreeDirectionKey)){
				itemCurr = m_TreeCtrl.InsertItem(wstrDirectionKey, itemState);
			}
		}
		//////////////////////////////////////////////////////////////////////

		//����
		fin.getline(iColorA, MAX_STR, '|');
		fin.getline(iColorR, MAX_STR, '|');
		fin.getline(iColorG, MAX_STR, '|');
		fin.getline(iColorB, MAX_STR, '|');

		//�ؽ��� �߽ɰ�
		fin.getline(iCenterX, MAX_STR, '|');
		fin.getline(iCenterY, MAX_STR, '|');

		//�ؽ��� ���� ���
		fin.getline(wstrImgPath, MAX_STR);

		/*if(fin.eof())
			break;*/

		//�о�� ������ ���� ����ü�� �����Ͽ� ����
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

		//Ʈ�� ��Ʈ���� �����ۿ� ������ ����(DWORD_PTR�� ����ȯ �Ͽ� ����)
		m_TreeCtrl.SetItemData(itemCurr, reinterpret_cast<DWORD_PTR>(pPathInfo));

		//�����ϱ� ���ϰ� ����Ʈ�� ���� �� ����, ��� �������� ����
		m_InfoList.push_back(pPathInfo);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

//Ʈ�� ��Ʈ���� �׸� ���ý� �߻��ϴ� �̺�Ʈ ó����
void CSpriteTool::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult){
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	//Ÿ�̸� ����
	KillTimer(1);

	//���õ� Ʈ�� ��Ʈ���� �׸� �ش��ϴ� �����͸� ������(DWORD_PTR������ ��ȯ�Ǿ� �ֱ� ������ �ٽ� PATH_INFO*�� ����ȯ��)
	PATH_INFO* pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

	//������ ������ ������ return
	if(nullptr == pInfo)
		return;

	//////////////////////////////////////////////////////////////////////
	//������ �����͸� �� ��Ʈ�ѿ� �Է�

	//�߽���
	m_iCenterX = pInfo->iCenterX;
	m_iCenterY = pInfo->iCenterY;

	//����
	m_ColorCtrl.SetColor(RGB(pInfo->iColorR, pInfo->iColorG, pInfo->iColorB));

	//�����̴� ����
	m_SliderCtrl.SetRange(0, pInfo->iImgCount - 1);

	//////////////////////////////////////////////////////////////////////

	//�̸�����

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(pInfo->wstrTypeKey, pInfo->wstrObjectKey, 0, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	Render(pTextureInfo);

	//////////////////////////////////////////////////////////////////////

	UpdateData(FALSE);

	*pResult = 0;
}

//���� ��ư Ŭ���� �߻��ϴ� �̺�Ʈ ó����
void CSpriteTool::OnBnClickedApply(){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	//////////////////////////////////////////////////////////////////////

	//Ÿ�̸� ����
	KillTimer(1);

	//���õ� Ʈ�� ��Ʈ���� �׸� �ش��ϴ� �����͸� ������(DWORD_PTR������ ��ȯ�Ǿ� �ֱ� ������ �ٽ� PATH_INFO*�� ����ȯ��)
	PATH_INFO* pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

	//������ ������ ������ return
	if(nullptr == pInfo)
		return;

	COLORREF tempColor = m_ColorCtrl.GetColor();

	//����ó���� ���� RGB�� ���� ����(�ݴ�� BGR�� �Ǿ�����)
	int iR = (tempColor & 0x000000FF);
	int iG = (tempColor & 0x0000FF00) >> 8;
	int iB = (tempColor & 0x00FF0000) >> 16;

	D3DCOLOR transColor = D3DCOLOR_ARGB(255, iR, iG, iB);

	//�߽����� D3DXVECTOR3�� ��ȯ
	D3DXVECTOR3 vec3Center = { static_cast<float>(m_iCenterX), static_cast<float>(m_iCenterY), 0.f };

	//�߽��� ����
	m_pTextureMgr->SetTextureCenter(vec3Center, pInfo->wstrTypeKey, pInfo->wstrObjectKey, pInfo->wstrStateKey, pInfo->wstrDirectionKey);
	//����� ����
	m_pTextureMgr->SetTextureTrans(transColor, pInfo->wstrTypeKey, pInfo->wstrObjectKey, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	//�� ������ ���� ����
	pInfo->iCenterX = m_iCenterX;
	pInfo->iCenterY = m_iCenterY;
	pInfo->iColorA = (transColor & 0xFF000000) >> 24;
	pInfo->iColorR = iR;
	pInfo->iColorG = iG;
	pInfo->iColorB = iB;

	//////////////////////////////////////////////////////////////////////

	//�̸�����

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(pInfo->wstrTypeKey, pInfo->wstrObjectKey, 0, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	Render(pTextureInfo);

	UpdateData(FALSE);
}

//�����̵带 Ŭ�������� �߻��ϴ� �̺�Ʈ ó����
void CSpriteTool::OnNMReleasedcaptureSlider1(NMHDR *pNMHDR, LRESULT *pResult){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	//���õ� Ʈ�� ��Ʈ���� �׸� �ش��ϴ� �����͸� ������(DWORD_PTR������ ��ȯ�Ǿ� �ֱ� ������ �ٽ� PATH_INFO*�� ����ȯ��)
	PATH_INFO* pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

	//������ ������ ������ return
	if(nullptr == pInfo)
		return;

	int index = m_SliderCtrl.GetPos();

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(pInfo->wstrTypeKey, pInfo->wstrObjectKey, index, pInfo->wstrStateKey, pInfo->wstrDirectionKey);

	Render(pTextureInfo);

	*pResult = 0;
}

//��� ��ư Ŭ���� �߻��ϴ� �̺�Ʈ ó����
void CSpriteTool::OnBnClickedPlay(){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	SetTimer(1, 33, NULL);
}

//���� ��ư Ŭ���� �߻��ϴ� �̺�Ʈ ó����
void CSpriteTool::OnBnClickedStop(){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	KillTimer(1);
}

//Ÿ�̸� �̺�Ʈ ó����
void CSpriteTool::OnTimer(UINT_PTR nIDEvent){
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	const TEXTURE_INFO* pTextureInfo = nullptr;
	PATH_INFO* pInfo = nullptr;

	switch(nIDEvent){
	case 1:
		pInfo = reinterpret_cast<PATH_INFO*>(m_TreeCtrl.GetItemData(m_TreeCtrl.GetSelectedItem()));

		if(nullptr == pInfo)
			break;

		// ����� �ε��� �� ����
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

//���� ��ư Ŭ���� �߻��ϴ� �̺�Ʈ ó����
void CSpriteTool::OnBnClickedSaveSetting(){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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

//�̸����� ���� �Լ�
void CSpriteTool::Render(const TEXTURE_INFO * pTextureInfo){
	if(nullptr == pTextureInfo)
		return;

	//��� �ؽ����� ũ�� ������
	m_iTextureHeight = pTextureInfo->tImgInfo.Height;
	m_iTextureWidth = pTextureInfo->tImgInfo.Width;

	D3DXMATRIX matScale, matTrans, matWorld;
	//�̸����� â�� �� ä���� ǥ���ϱ� ���� ũ�� ����
	D3DXMatrixScaling(&matScale, static_cast<float>(WINCX) / pTextureInfo->tImgInfo.Width, static_cast<float>(WINCY) / pTextureInfo->tImgInfo.Height, 0.f);
	//�̸����� â�� ����� �̵�
	D3DXMatrixTranslation(&matTrans, WINCX*0.5f, WINCY*0.5f, 0.f);

	//���� ��� = ũ�� ��� * ���� ��� * �̵� ��� * ���� ��� * �θ� ��� 
	matWorld = matScale* matTrans;
	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

	//���� �غ�
	m_pDeviceMgr->Render_Begin();
	//�ĸ���ۿ� Draw
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture,
		nullptr,
		&pTextureInfo->vec3CenterPoint,
		nullptr,
		D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pDeviceMgr->GetSprite()->End();

	//�߽����� ǥ���� ���ڰ�
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

