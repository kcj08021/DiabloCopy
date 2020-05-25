// MyFormView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "MyFormView.h"
#include "MainFrm.h"
#include "MapTool.h"
#include "ObjectTool.h"
#include "PathTool.h"
#include "SpriteTool.h"




// CMyFormView

IMPLEMENT_DYNCREATE(CMyFormView, CFormView)

CMyFormView::CMyFormView()
	: CFormView(IDD_MYFORMVIEW), m_pMapTool(nullptr), m_pObjectTool(nullptr)
{

}

CMyFormView::~CMyFormView()
{
	m_pMapTool->DestroyWindow();
	m_pObjectTool->DestroyWindow();
	m_pPathTool->DestroyWindow();
	m_pSpriteTool->DestroyWindow();

	SafeDelete(m_pMapTool);
	SafeDelete(m_pObjectTool);
	SafeDelete(m_pPathTool);
	SafeDelete(m_pSpriteTool);
}

void CMyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_MyTabControl);
}

BEGIN_MESSAGE_MAP(CMyFormView, CFormView)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CMyFormView::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CMyFormView �����Դϴ�.

#ifdef _DEBUG
void CMyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyFormView �޽��� ó�����Դϴ�.


void CMyFormView::OnInitialUpdate(){
	CFormView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, CSize(0,0));

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_MyTabControl.InsertItem(0, L"Ÿ��");
	m_MyTabControl.InsertItem(1, L"������Ʈ");
	m_MyTabControl.InsertItem(2, L"��������Ʈ");
	m_MyTabControl.InsertItem(3, L"��λ���");

	m_MyTabControl.SetCurSel(0);

	RECT rc = {};

	GetClientRect(&rc);

	m_MyTabControl.SetWindowPos(nullptr, 5, 5, rc.right-10, rc.bottom-10, SWP_SHOWWINDOW|SWP_NOZORDER);

	m_pMapTool = new CMapTool;
	m_pMapTool->Create(IDD_MAPTOOL, &m_MyTabControl);
	m_pMapTool->MoveWindow(0, 20, rc.right-10, rc.bottom-30);
	m_pMapTool->ShowWindow(SW_SHOW);

	m_pObjectTool = new CObjectTool;
	m_pObjectTool->Create(IDD_OBJECTTOOL, &m_MyTabControl);
	m_pObjectTool->MoveWindow(0, 20, rc.right-10, rc.bottom-30);
	m_pObjectTool->ShowWindow(SW_HIDE);

	m_pPathTool = new CPathTool;
	m_pPathTool->Create(IDD_PATHTOOL, &m_MyTabControl);
	m_pPathTool->MoveWindow(0, 20, rc.right-10, rc.bottom-30);
	m_pPathTool->ShowWindow(SW_HIDE);

	m_pSpriteTool = new CSpriteTool;
	m_pSpriteTool->Create(IDD_SPRITETOOL, &m_MyTabControl);
	m_pSpriteTool->MoveWindow(0, 20, rc.right - 10, rc.bottom - 30);
	m_pSpriteTool->ShowWindow(SW_HIDE);
}

void CMyFormView::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult){
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	int iSelect = m_MyTabControl.GetCurSel();

	switch(iSelect){
	case 0:		//����
		m_pMapTool->ShowWindow(SW_SHOW);
		m_pObjectTool->ShowWindow(SW_HIDE);
		m_pPathTool->ShowWindow(SW_HIDE);
		m_pSpriteTool->ShowWindow(SW_HIDE);
		break;
	case 1:		//������Ʈ��
		m_pMapTool->ShowWindow(SW_HIDE);
		m_pObjectTool->ShowWindow(SW_SHOW);
		m_pPathTool->ShowWindow(SW_HIDE);
		m_pSpriteTool->ShowWindow(SW_HIDE);
		break;
	case 2:
		m_pMapTool->ShowWindow(SW_HIDE);
		m_pObjectTool->ShowWindow(SW_HIDE);
		m_pPathTool->ShowWindow(SW_HIDE);
		m_pSpriteTool->ShowWindow(SW_SHOW);
		break;
	case 3:
		m_pMapTool->ShowWindow(SW_HIDE);
		m_pObjectTool->ShowWindow(SW_HIDE);
		m_pPathTool->ShowWindow(SW_SHOW);
		m_pSpriteTool->ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}

	*pResult = 0;
}