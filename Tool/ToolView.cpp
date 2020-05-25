
// ToolView.cpp : CToolView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "Terrain.h"
#include "ColTile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CScrollView)

BEGIN_MESSAGE_MAP(CToolView, CScrollView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

HWND g_hWnd;

// CToolView 생성/소멸

CToolView::CToolView(): m_pDeviceMgr(DEVICEMGR), m_pTextureMgr(TEXTUREMGR), m_pTerrain(nullptr), m_pColTile(nullptr), m_iIndex(-1), m_bIsColPicking(false)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView(){
	SafeDelete(m_pTerrain);
	SafeDelete(m_pColTile);

	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView 그리기

void CToolView::SetTileGrid(BOOL _bIsGrid){
	m_pTerrain->SetTileGrid(_bIsGrid);

	Invalidate(FALSE);
}

void CToolView::SetColGrid(BOOL _bIsColGrid){
	m_pColTile->SetColGrid(_bIsColGrid);

	Invalidate(FALSE);
}

void CToolView::SetDisplayTileIndex(BOOL _bIsDisplay){
	m_pTerrain->SetDisplayIndex(_bIsDisplay);

	Invalidate(FALSE);
}

void CToolView::SetDisplayColIndex(BOOL _bIsDisplay){
	m_pColTile->SetDisplayIndex(_bIsDisplay);

	Invalidate(FALSE);
}

void CToolView::OnDraw(CDC* /*pDC*/){
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.

	m_pDeviceMgr->Render_Begin();

	m_pTerrain->Render();

	m_pColTile->Render();

	m_pDeviceMgr->Render_End();
}


// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo){
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기


void CToolView::OnInitialUpdate(){
	CScrollView::OnInitialUpdate();

	//스크롤 초기화(스크롤뷰로 상속받았을때 반드시 스크롤값을 초기화 해줘야 한다.)
	CScrollView::SetScrollSizes(MM_TEXT, CSize(TILECX*TILEX,TILECY*TILEY));
	CScrollView::SetScrollPos(0, (TILECX*TILEX)>>1);


	//View 사이즈 변경
	//프레임 윈도우 얻어오기(메인 프레임)
	//AfxGetApp()함수는 CWinApp의 포인터를 얻어오는 MFC 전역 함수.
	//GetMainWnd()함수는 MainFrm의 포인터를 얻어오는 CWinApp의 멤버 함수.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if(nullptr == pMainFrm){
		::MessageBox(g_hWnd, L"CToolView::pMainFrm is nullptr", L"Error", MB_OK);
	}

	RECT rcMainFrm = {};
	//GetWindowRect()함수는 윈도우의 테두리를 포함한 전체크기를 얻어오는 함수.
	//얻어오는 RECT의 좌표는 스크린 기준의 좌표이다.
	pMainFrm->GetWindowRect(&rcMainFrm);

	//사각형 계산
	::SetRect(&rcMainFrm, 0, 0, rcMainFrm.right - rcMainFrm.left, rcMainFrm.bottom - rcMainFrm.top);


	RECT rcView = {};
	//GetClientRect()함수는 테두리 같은것을 제외한 순수 클라이언트의 크기를 얻어오는 함수.
	//이 함수로 얻어오는 RECT의 좌표는 클라이언트 좌표(left와 top은 항상 0)
	GetClientRect(&rcView);

	int iGapX = rcMainFrm.right - rcView.right;
	int iGapY = rcMainFrm.bottom - rcView.bottom;

	//SetWindowPos()함수는 MainFrm의 크기를 조정하는 함수
	pMainFrm->SetWindowPos(nullptr, 100, 100, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	g_hWnd = m_hWnd;

	HRESULT hr = 0;

	hr = m_pDeviceMgr->InitDevice(CDeviceManager::MODE_WINDOWED);
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"InitDevice Failed", L"Error", MB_OK);
	}

	hr = m_pTextureMgr->LoadTextureFromPathFile(L"../Data/TextruePath.txt");
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"Texture Load From PathFile Failed", L"Error", MB_OK);
	}
	m_pTerrain = CTerrain::Create(this);
	if(nullptr == m_pTerrain){
		::MessageBox(g_hWnd, L"Terrain Create Failed", L"Error", MB_OK);
	}

	m_pColTile = CColTile::Create(this);
	if(nullptr == m_pColTile){
		::MessageBox(g_hWnd, L"Terrain Create Failed", L"Error", MB_OK);
	}
}


void CToolView::OnLButtonDown(UINT nFlags, CPoint point){
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CScrollView::OnLButtonDown(nFlags, point);

	if(m_bIsColPicking)
		m_pColTile->TilePicking(point);
	else
		m_pTerrain->TilePicking(point, m_iIndex);

	Invalidate(false);
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point){
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CScrollView::OnMouseMove(nFlags, point);

	if(::GetAsyncKeyState(VK_LBUTTON) & 0x8000){
		if(m_bIsColPicking)
			m_pColTile->TilePicking(point);
		else
			m_pTerrain->TilePicking(point, m_iIndex);
	}

	Invalidate(false);
}


void CToolView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int iScrollX = GetScrollPos(0);
	int iScrollY = GetScrollPos(1);

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
	if(GetAsyncKeyState(VK_UP) & 0x8000)
		iScrollY -= 30;
	if(GetAsyncKeyState(VK_DOWN) & 0x8000)
		iScrollY += 30;
	if(GetAsyncKeyState(VK_LEFT) & 0x8000)
		iScrollX -= 30;
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
		iScrollX += 30;

	SetScrollPos(0, iScrollX);
	SetScrollPos(1, iScrollY);

	Invalidate(FALSE);
}
