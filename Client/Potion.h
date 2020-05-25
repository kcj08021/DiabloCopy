#ifndef __POTION_H__
#define __POTION_H__

#include "Item.h"
class CPlayer;

class CPotion:
	public CItem{
protected:
	CPlayer* m_pPlayer;

public:
	explicit CPotion();
	virtual ~CPotion();

public:
	virtual void PotionUsed()=0;
};

#endif // !__POTION_H__

