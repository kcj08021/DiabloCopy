#ifndef __LIGHTNING_H__
#define __LIGHTNING_H__

#include "Spell.h"

class CScrollManager;

class CLightning:
	public CSpell{
private:
	CScrollManager* m_pScrollMgr;

private:
	explicit CLightning();
public:
	virtual ~CLightning();

	// CSpell을(를) 통해 상속됨
public:
	virtual HRESULT Use(const D3DXVECTOR3 _vPos) const override;
public:
	static CLightning* Create(CPlayer* _pPlayer);
};

#endif // !__LIGHTNING_H__
