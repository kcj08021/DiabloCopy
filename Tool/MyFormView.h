#pragma once
#include "afxcmn.h"



// CMyFormView 폼 뷰입니다.
class CMapTool;
class CObjectTool;
class CPathTool;
class CSpriteTool;


class CMyFormView : public CFormView
{
	DECLARE_DYNCREATE(CMyFormView)

protected:
	CMyFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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


