
// ToolView.h : CToolView 클래스의 인터페이스
//

#pragma once
class CToolDoc;

class CDeviceManager;
class CTextureManager;
class CTerrain;
class CColTile;
class CPathTool;

class CToolView : public CScrollView
{
protected: // serialization에서만 만들어집니다.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// 멤버 변수
private:
	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;
	CTerrain* m_pTerrain;
	CColTile* m_pColTile;
	int m_iIndex;
	bool m_bIsColPicking;

// 특성입니다.
public:
	CToolDoc* GetDocument() const;
	CTerrain* GetTerrain(){ return m_pTerrain; }
	CColTile* GetColTile(){ return m_pColTile; }
// 작업입니다.
public:
	void SetTileGrid(BOOL _bIsGrid);
	void SetColGrid(BOOL _bIsColGrid);
	void SetDisplayTileIndex(BOOL _bIsDisplay);
	void SetDisplayColIndex(BOOL _bIsDisplay);
	void SetIndex(int _iIndex){ m_iIndex = _iIndex; }
	void SetColTile(bool _bool){ m_bIsColPicking = _bool; }
// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

