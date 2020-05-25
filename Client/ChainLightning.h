#ifndef __CHAIN_LIGHTNING_H__
#define __CHAIN_LIGHTNING_H__
#include "Spell.h"

class CObjectManager;
class CScrollManager;

class CChainLightning:
	public CSpell{
private:
	CObjectManager* m_pObjMgr;
	CScrollManager* m_pScrollMgr;
public:
	explicit CChainLightning();
	virtual ~CChainLightning();
	
public:	// CSpell을(를) 통해 상속됨
	virtual HRESULT Use(const D3DXVECTOR3 _vPos) const override;

public:
	static CChainLightning* Create(CPlayer* _pPlayer);
};

#endif // !__CHAIN_LIGHTNING_H__

