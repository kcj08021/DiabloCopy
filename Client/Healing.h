#ifndef __HEALING_H__
#define __HEALING_H__

#include "Spell.h"
class CHealing:
	public CSpell{
public:
	explicit CHealing();
	virtual ~CHealing();


	// CSpell��(��) ���� ��ӵ�
public:
	virtual HRESULT Use(const D3DXVECTOR3 _vPos) const override;

	static CHealing* Create(CPlayer* _pPlayer);
};

#endif // !__HEALING_H__



