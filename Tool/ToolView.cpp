
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
// SHARED_HANDLERS�� �̸� ����, ����� �׸� �� �˻� ���� ó���⸦ �����ϴ� ATL ������Ʈ���� ������ �� ������
// �ش� ������Ʈ�� ���� �ڵ带 �����ϵ��� �� �ݴϴ�.
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
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

HWND g_hWnd;

// CToolView ����/�Ҹ�

CToolView::CToolView(): m_pDeviceMgr(DEVICEMGR), m_pTextureMgr(TEXTUREMGR), m_pTerrain(nullptr), m_pColTile(nullptr), m_iIndex(-1), m_bIsColPicking(false)
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.

}

CToolView::~CToolView(){
	SafeDelete(m_pTerrain);
	SafeDelete(m_pColTile);

	m_pTextureMgr->DestroyInstance();
	m_pDeviceMgr->DestroyInstance();
}

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs){
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CScrollView::PreCreateWindow(cs);
}

// CToolView �׸���

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

	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.

	m_pDeviceMgr->Render_Begin();

	m_pTerrain->Render();

	m_pColTile->Render();

	m_pDeviceMgr->Render_End();
}


// CToolView �μ�

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo){
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}


// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const{
	CScrollView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const{
	CScrollView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����


void CToolView::OnInitialUpdate(){
	CScrollView::OnInitialUpdate();

	//��ũ�� �ʱ�ȭ(��ũ�Ѻ�� ��ӹ޾����� �ݵ�� ��ũ�Ѱ��� �ʱ�ȭ ����� �Ѵ�.)
	CScrollView::SetScrollSizes(MM_TEXT, CSize(TILECX*TILEX,TILECY*TILEY));
	CScrollView::SetScrollPos(0, (TILECX*TILEX)>>1);


	//View ������ ����
	//������ ������ ������(���� ������)
	//AfxGetApp()�Լ��� CWinApp�� �����͸� ������ MFC ���� �Լ�.
	//GetMainWnd()�Լ��� MainFrm�� �����͸� ������ CWinApp�� ��� �Լ�.
	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(::AfxGetApp()->GetMainWnd());
	if(nullptr == pMainFrm){
		::MessageBox(g_hWnd, L"CToolView::pMainFrm is nullptr", L"Error", MB_OK);
	}

	RECT rcMainFrm = {};
	//GetWindowRect()�Լ��� �������� �׵θ��� ������ ��üũ�⸦ ������ �Լ�.
	//������ RECT�� ��ǥ�� ��ũ�� ������ ��ǥ�̴�.
	pMainFrm->GetWindowRect(&rcMainFrm);

	//�簢�� ���
	::SetRect(&rcMainFrm, 0, 0, rcMainFrm.right - rcMainFrm.left, rcMainFrm.bottom - rcMainFrm.top);


	RECT rcView = {};
	//GetClientRect()�Լ��� �׵θ� �������� ������ ���� Ŭ���̾�Ʈ�� ũ�⸦ ������ �Լ�.
	//�� �Լ��� ������ RECT�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ(left�� top�� �׻� 0)
	GetClientRect(&rcView);

	int iGapX = rcMainFrm.right - rcView.right;
	int iGapY = rcMainFrm.bottom - rcView.bottom;

	//SetWindowPos()�Լ��� MainFrm�� ũ�⸦ �����ϴ� �Լ�
	pMainFrm->SetWindowPos(nullptr, 100, 100, WINCX + iGapX, WINCY + iGapY, SWP_NOZORDER);

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CScrollView::OnLButtonDown(nFlags, point);

	if(m_bIsColPicking)
		m_pColTile->TilePicking(point);
	else
		m_pTerrain->TilePicking(point, m_iIndex);

	Invalidate(false);
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point){
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
