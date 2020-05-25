#ifndef __FIREWALL_EFFECT_H__
#define __FIREWALL_EFFECT_H__

#include "GameObject.h"

class CColTile;
class CObjectManager;

class CFireWallEffect:
	public CGameObject{
public:
	enum HORI_VERT{ HORIZONAL, VERTICAL };
	enum SON_SPEC{ NONE, UP, DOWN};

private:
	HORI_VERT m_eDirection;
	SON_SPEC m_eIsSon;
	CColTile* m_pColTile;
	float m_fHoldingTime;
	float m_pDelayTime;
	int m_iRecur;

	CObjectManager* m_pObjMgr;

public:
	explicit CFireWallEffect();
	virtual ~CFireWallEffect();

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

public:
	static CFireWallEffect* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile, HORI_VERT _enum);
};

#endif // !__FIREWALL_EFFECT_H__

