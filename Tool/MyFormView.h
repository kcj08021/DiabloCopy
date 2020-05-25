#pragma once
#include "afxcmn.h"



// CMyFormView �� ���Դϴ�.
class CMapTool;
class CObjectTool;
class CPathTool;
class CSpriteTool;


class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMyFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYFORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	CTabCtrl m_MyTabControl;
	CMapTool* m_pMapTool;
	CObjectTool* m_pObjectTool;
	CPathTool* m_pPathTool;
	CSpriteTool* m_pSpriteTool;

public:
	virtual void OnInitialUpdate();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};


