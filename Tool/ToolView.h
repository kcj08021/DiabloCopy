
// ToolView.h : CToolView Ŭ������ �������̽�
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
protected: // serialization������ ��������ϴ�.
	CToolView();
	DECLARE_DYNCREATE(CToolView)

	// ��� ����
private:
	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;
	CTerrain* m_pTerrain;
	CColTile* m_pColTile;
	int m_iIndex;
	bool m_bIsColPicking;

// Ư���Դϴ�.
public:
	CToolDoc* GetDocument() const;
	CTerrain* GetTerrain(){ return m_pTerrain; }
	CColTile* GetColTile(){ return m_pColTile; }
// �۾��Դϴ�.
public:
	void SetTileGrid(BOOL _bIsGrid);
	void SetColGrid(BOOL _bIsColGrid);
	void SetDisplayTileIndex(BOOL _bIsDisplay);
	void SetDisplayColIndex(BOOL _bIsDisplay);
	void SetIndex(int _iIndex){ m_iIndex = _iIndex; }
	void SetColTile(bool _bool){ m_bIsColPicking = _bool; }
// �������Դϴ�.
public:
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// �����Դϴ�.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // ToolView.cpp�� ����� ����
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

