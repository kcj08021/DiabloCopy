#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CMapTool 대화 상자입니다.

class CToolView;

class CMapTool : public CDialogEx
{
	DECLARE_DYNAMIC(CMapTool)

public:
	CMapTool(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMapTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MAPTOOL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// 타일 리스트 변수
	CListBox m_ListBox;
	// 미리보기가 표시될 컨트롤
	CStatic m_Preview;
	// 타일 그리드 표시 여부
	CButton m_CheckTileGrid;
	// 충돌 그리드 표시 여부
	CButton m_CheckColGrid;
	//타일 인덱스 표시 여부
	CButton m_CheckTileIndex;
	// 충돌 인덱스 표시 여부
	CButton m_CheckColIndex;

	CToolView* m_pToolView;

public:
	afx_msg void OnLbnSelchangeList4();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();

	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	// 타일 인덱스 표시 여부

	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedMapTileSave();
	afx_msg void OnBnClickedMapTileLoad();
	afx_msg void OnBnClickedColTileSave();
	afx_msg void OnBnClickedColTileLoad();
};
