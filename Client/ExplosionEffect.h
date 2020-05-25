#ifndef __EXPLOSION_EFFECT_H__
#define __EXPLOSION_EFFECT_H__

#include "GameObject.h"
class CColTile;

class CExplosionEffect:
	public CGameObject{
private:
	CColTile* m_pColTile;

private:
	explicit CExplosionEffect();
public:
	virtual ~CExplosionEffect();

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
	static CExplosionEffect* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile);
};

#endif // !__EXPLOSION_EFFECT_H__

