#pragma once
#include "afxwin.h"


// CPathTool 대화 상자입니다.

class CPathTool : public CDialog
{
	DECLARE_DYNAMIC(CPathTool)
private:								//사용자 정의 멤버 변수
	list<PATH_INFO*> m_PathList;
	CListBox m_PathListBox;

public:
	CPathTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPathTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPathSave();
	afx_msg void OnBnClickedPathLoad();


private:
	void ExtractPathInfo(const TCHAR* _szPath);
	size_t CountImgFiles(const TCHAR* _szPath);
	void LoadPathInfoFile();
	void InitHorizontalScroll();

public:
	virtual BOOL OnInitDialog();
};
