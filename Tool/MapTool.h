#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMapTool ��ȭ �����Դϴ�.

class CToolView;

class CMapTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMapTool();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// Ÿ�� ����Ʈ ����
	CListBox m_ListBox;
	// �̸����Ⱑ ǥ�õ� ��Ʈ��
	CStatic m_Preview;
	// Ÿ�� �׸��� ǥ�� ����
	CButton m_CheckTileGrid;
	// �浹 �׸��� ǥ�� ����
	CButton m_CheckColGrid;
	//Ÿ�� �ε��� ǥ�� ����
	CButton m_CheckTileIndex;
	// �浹 �ε��� ǥ�� ����
	CButton m_CheckColIndex;

	CToolView* m_pToolView;

public:
	afx_msg void OnLbnSelchangeList4();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();

	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	// Ÿ�� �ε��� ǥ�� ����

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedMapTileSave();
	afx_msg void OnBnClickedMapTileLoad();
	afx_msg void OnBnClickedColTileSave();
	afx_msg void OnBnClickedColTileLoad();
};
