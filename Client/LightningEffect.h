#ifndef __LIGHTNING_EFFECT_H__
#define __LIGHTNING_EFFECT_H__

#include "GameObject.h"

class CObjectManager;
class CColTile;

class CLightningEffect:
	public CGameObject{
public:
	enum SKILL_TYPE{ NORMAL, CHAIN};

private:
	D3DXVECTOR3 m_vTempPos;
	RECT m_tTempRect;
	int m_iRecur;
	bool m_bIsCreateSon;
	CColTile* m_pColTile;
	const CGameObject* m_pDamagedObj;

	CObjectManager* m_pObjMgr;

public:
	explicit CLightningEffect();
	virtual ~CLightningEffect();

private:
	void UpdateRect();

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
	static CLightningEffect* Create(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, CColTile* _pColTile, SKILL_TYPE eType);
};

#endif // !__LIGHTNING_EFFECT_H__

