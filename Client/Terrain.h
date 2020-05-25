#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include "GameObject.h"

	// 전방 선언
class CDeviceManager;
class CTextureManager;
class CScrollManager;
class CObjectManager;

class CTerrain: 
	public CGameObject{
	// 멤버 변수
private:
	vector<TILE_INFO*> m_vecTile;
	vector<bool> m_vecTileVisible;

	BOOL m_bIsTileGrid;
	BOOL m_bIsDisplayIndex;
	bool m_bIsMiniMap;

	CObjectManager* m_pObjMgr;
	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;
	CScrollManager* m_pScrollMgr;

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
	void SetMiniMap();
	// 일반 멤버 함수
public:
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
	static CTerrain* Create();
};

#endif // !__TERRAIN_H__

