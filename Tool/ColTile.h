#ifndef __COL_TILE_H__
#define __COL_TILE_H__
#include "GameObject.h"

class CDeviceManager;
class CToolView;

class CColTile:
	public CGameObject{
private:
	vector<COL_INFO*> m_vecTile;
	CDeviceManager* m_pDeviceMgr;
	CToolView* m_pView;
	BOOL m_bIsColGird;
	BOOL m_bIsDisplayIndex;

private:
	explicit CColTile();
public:
	virtual ~CColTile();

private:
	int GetIndex(D3DXVECTOR3& _vPos);

public:
	void SetColGrid(BOOL _bIsColGrid){ m_bIsColGird = _bIsColGrid; }
	void SetDisplayIndex(BOOL _bIsDisplay){ m_bIsDisplayIndex = _bIsDisplay; }
public:
	void TilePicking(const CPoint& _pt); 
	void SaveTile(const TCHAR* _szFilePath);
	void LoadTile(const TCHAR* _szFilePath);

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

	static CColTile* Create(CToolView* _pView);
};

#endif // !__COL_TILE_H__

