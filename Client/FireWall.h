#ifndef __FIREWALL_H__
#define __FIREWALL_H__

#include "Spell.h"

class CColTile;
class CScrollManager;

class CFireWall:
	public CSpell{
private:
	CScrollManager* m_pScrollMgr;

private:
	explicit CFireWall();
public:
	virtual ~CFireWall();

	// CSpell��(��) ���� ��ӵ�
public:
	virtual HRESULT Use(const D3DXVECTOR3 _vPos) const override;

public:
	static CFireWall* Create(CPlayer * _pPlayer);
};

#endif // !__FIREWALL_H__
