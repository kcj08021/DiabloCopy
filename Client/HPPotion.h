#ifndef __HP_POTION_H__
#define __HP_POTION_H__

#include "Potion.h"
class CHPPotion:
	public CPotion{
private:
	explicit CHPPotion();
public:
	virtual ~CHPPotion();

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
	static CHPPotion* Create(const D3DXVECTOR3& _vPos, CPlayer* _pPlayer);


};

#endif // !__HP_POTION_H__

