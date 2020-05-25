// MapTool.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapTool.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "Terrain.h"
#include "ColTile.h"


// CMapTool 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapTool, CDialogEx)

CMapTool::CMapTool(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAPTOOL, pParent){
}

CMapTool::~CMapTool()
{
}

void CMapTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST4, m_ListBox);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_Preview);
	DDX_Control(pDX, IDC_CHECK1, m_CheckTileGrid);
	DDX_Control(pDX, IDC_CHECK2, m_CheckColGrid);
	DDX_Control(pDX, IDC_CHECK3, m_CheckTileIndex);
	DDX_Control(pDX, IDC_CHECK4, m_CheckColIndex);

}


BEGIN_MESSAGE_MAP(CMapTool, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST4, &CMapTool::OnLbnSelchangeList4)
	ON_BN_CLICKED(IDC_CHECK1, &CMapTool::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CMapTool::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CMapTool::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CMapTool::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_RADIO1, &CMapTool::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CMapTool::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapTool::OnBnClickedMapTileSave)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapTool::OnBnClickedMapTileLoad)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapTool::OnBnClickedColTileSave)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapTool::OnBnClickedColTileLoad)
END_MESSAGE_MAP()


// CMapTool 메시지 처리기입니다.


BOOL CMapTool::OnInitDialog(){
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CFileFind find;

	BOOL bIsFind = find.FindFile(L"../Texture/Map/Tile/*.*");

	while(bIsFind){
		bIsFind = find.FindNextFile();

		if(find.IsDots() || find.IsDirectory() || find.IsSystem())
			continue;
		TCHAR szString[MIN_STR] = {};
		lstrcpy(szString, find.GetFileName().GetString());
		::PathRemoveExtension(szString);

		m_ListBox.AddString(szString);
	}

	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	m_pToolView = pMainFrm->GetToolView();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CMapTool::OnLbnSelchangeList4(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iSelect = m_ListBox.GetCurSel();

	if(-1 == iSelect)
		return;

	CString strSelectName = L"";
	m_ListBox.GetText(iSelect, strSelectName);

	int i = 0;

	for(; i < strSelectName.GetLength(); ++i){
		if(isdigit(strSelectName[i]))
			break;
	}

	strSelectName.Delete(0, i);

	int iDrawIndex = _ttoi(strSelectName);


	m_pToolView->SetIndex(iDrawIndex);

	//미리보기
	const TEXTURE_INFO* pTexInfo = TEXTUREMGR->GetTextureInfo(
		L"Map", L"Tile" , iDrawIndex);
	if(nullptr == pTexInfo)
		return;

	DEVICEMGR->Render_Begin();

	D3DXMATRIX matScale, matTrans, matWorld;
	D3DXMatrixScaling(&matScale, float(WINCX) / pTexInfo->tImgInfo.Width, float(WINCY) / pTexInfo->tImgInfo.Height, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);

	matWorld = matScale * matTrans;

	DEVICEMGR->GetSprite()->SetTransform(&matWorld);
	DEVICEMGR->GetSprite()->Draw(pTexInfo->pTexture,
		nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	
	DEVICEMGR->Render_End(m_Preview.m_hWnd);
}


void CMapTool::OnBnClickedCheck1(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pToolView->SetTileGrid(m_CheckTileGrid.GetCheck());

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedCheck2(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_pToolView->SetColGrid(m_CheckColGrid.GetCheck());

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedCheck3(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pToolView->SetDisplayTileIndex(m_CheckTileIndex.GetCheck());

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedCheck4(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_pToolView->SetDisplayColIndex(m_CheckColIndex.GetCheck());
	
	UpdateData(FALSE);
}


void CMapTool::OnBnClickedRadio1(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pToolView->SetColTile(false);
}


void CMapTool::OnBnClickedRadio2(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pToolView->SetColTile(true);
}


void CMapTool::OnBnClickedMapTileSave(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(FALSE, L".mapdat", L"World.mapdat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Map Data Files(*.mapdat)|*.mapdat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if(IDOK == Dlg.DoModal()){
		
		m_pToolView->GetTerrain()->SaveTile(Dlg.GetPathName());
	}

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedMapTileLoad(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(TRUE, L".mapdat", L"World.mapdat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Map Data Files(*.mapdat)|*.mapdat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if(IDOK == Dlg.DoModal()){
		m_pToolView->GetTerrain()->LoadTile(Dlg.GetPathName());
	}

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedColTileSave(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L".coldat", L"World.coldat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Collision Data Files(*.coldat)|*.coldat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if(IDOK == Dlg.DoModal()){

		m_pToolView->GetColTile()->SaveTile(Dlg.GetPathName());
	}

	UpdateData(FALSE);
}


void CMapTool::OnBnClickedColTileLoad(){
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	// 파일 열기 혹은 저장에 관한 대화상자 MFC 클래스
	CFileDialog Dlg(TRUE, L".coldat", L"World.coldat",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Map Data Files(*.coldat)|*.coldat|Text Files(*.txt)|*.txt||",
		this);

	TCHAR szCurrentPath[MAX_STR] = L"";

	// 현재 작업 경로를 얻어오는 함수.
	::GetCurrentDirectory(MAX_STR, szCurrentPath);

	// 현재 경로에서 파일명 제거하는 함수. 제거할 파일명이 없으면 말단 폴더명을 제거한다.
	::PathRemoveFileSpec(szCurrentPath);
	lstrcat(szCurrentPath, L"\\Data");

	// 현재 대화상자에서 보여질 초기 경로 설정.
	Dlg.m_ofn.lpstrInitialDir = szCurrentPath; // 상대경로 X

	if(IDOK == Dlg.DoModal()){
		m_pToolView->GetColTile()->LoadTile(Dlg.GetPathName());
	}

	UpdateData(FALSE);
}
