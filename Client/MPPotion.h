#ifndef __MP_POTION_H__
#define __MP_POTION_H__

#include "Potion.h"
class CMPPotion:
	public CPotion{
private:
	explicit CMPPotion();
public:
	virtual ~CMPPotion();

	// CPotion을(를) 통해 상속됨
public:
	virtual void PotionUsed() override;
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
public:
	static CMPPotion* Create(const D3DXVECTOR3& _vPos, CPlayer* _pPlayer);
};


#endif // !__MP_POTION_H__
