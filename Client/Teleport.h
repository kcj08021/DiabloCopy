#ifndef __TELEPORT_H__
#define __TELEPORT_H__

#include "Spell.h"
class CColTile;

class CTeleport:
	public CSpell{
private:
private:
	explicit CTeleport();
public:
	virtual ~CTeleport();

	// CSpell��(��) ���� ��ӵ�
public:
	virtual HRESULT Use(const D3DXVECTOR3 _vPos) const override;

	static CTeleport* Create(CPlayer* _pPlayer);
};

#endif // !__TELEPORT_H__

