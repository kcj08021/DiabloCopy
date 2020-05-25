#pragma once
#include "afxwin.h"


// CPathTool ��ȭ �����Դϴ�.

class CPathTool : public CDialog
{
	DECLARE_DYNAMIC(CPathTool)
private:								//����� ���� ��� ����
	list<PATH_INFO*> m_PathList;
	CListBox m_PathListBox;

public:
	CPathTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPathTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
