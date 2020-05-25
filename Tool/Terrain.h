#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "GameObject.h"

	// 전방 선언
class CDeviceManager;
class CTextureManager;
class CToolView;

class CTerrain: 
	public CGameObject{
	// 멤버 변수
private:
	vector<TILE_INFO*> m_vecTile;
	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;
	CToolView* m_pView;
	BOOL m_bIsTileGrid;
	BOOL m_bIsDisplayIndex;

	//생성자 및 소멸자
private:
	explicit CTerrain();
public:
	virtual ~CTerrain();

	// Get 함수
public:
	int GetIndex(const D3DXVECTOR3& _vPos) const;
public:
	void SetTileGrid(BOOL _bIsTileGrid){ m_bIsTileGrid = _bIsTileGrid; }
	void SetDisplayIndex(BOOL _bIsDisplay){ m_bIsDisplayIndex = _bIsDisplay; }
	// 일반 멤버 함수
public:
	void TilePicking(const CPoint& _pt, int _iIndex);
	void SaveTile(const TCHAR* _szFilePath);
	void LoadTile(const TCHAR* _szFilePath);

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

	// 정적 멤버 함수
public:
	static CTerrain* Create(CToolView* _pView);
};

#endif // !__TERRAIN_H__

