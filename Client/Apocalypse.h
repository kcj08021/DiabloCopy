#ifndef __APOCALYPSE_H__
#define __APOCALYPSE_H__

#include "Spell.h"
class CApocalypse:
	public CSpell{
private:
	CScrollManager* m_pScrollMgr;
private:
	explicit CApocalypse();
public:
	virtual ~CApocalypse();

	// CSpell을(를) 통해 상속됨
public:
	virtual HRESULT Use(const D3DXVECTOR3 _vPos) const override;

public:
	static CApocalypse* Create(CPlayer * _pPlayer);
};

#endif // !__APOCALYPSE_H__

