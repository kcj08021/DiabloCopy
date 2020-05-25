#ifndef __FIREBOLT_H__
#define __FIREBOLT_H__

#include "Spell.h"

class CGameObject;
class CScrollManager;

class CFireBolt:
	public CSpell{
	CScrollManager* m_pScrollMgr;

private:
	explicit CFireBolt();
public:
	virtual ~CFireBolt();

	// CSpell을(를) 통해 상속됨
public:
	virtual HRESULT Use(const D3DXVECTOR3 _vDir) const override;

public:
	static CFireBolt* Create(CPlayer* _pPlayer);
};

#endif // !__FIREBOLT_H__
