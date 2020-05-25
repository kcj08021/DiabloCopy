#ifndef __INVENTORY_PAGE_H__
#define __INVENTORY_PAGE_H__

#include "GameObject.h"

class CPlayer;
class CItem;
class CHUD;
class CKeyManager;

class CInventoryPage:
	public CGameObject{
	enum EQUIP_PART{HEAD, BODY, WEAPON, SHIELD, NECK, RING1, RING2, EQUIP_END};
	enum{INVEN_WIDTH = 10, INVEN_HIEGTH=4};
private:
	CPlayer* m_pPlayer;
	CHUD* m_pHUD;
	RECT m_tEquip[EQUIP_END];
	CItem* m_pEquipItem[EQUIP_END];

	RECT m_tInven[INVEN_HIEGTH][INVEN_WIDTH];
	CItem* m_InvenItem[INVEN_HIEGTH][INVEN_WIDTH];
	bool m_bIsRealItem[INVEN_HIEGTH][INVEN_WIDTH];

	CItem* m_pSelectedItem;
	bool m_bisItemSelected;

	bool m_bIsSoundOut;

	CKeyManager* m_pKeyMgr;

public:
	explicit CInventoryPage();
	virtual ~CInventoryPage();

private:
	void CreateEquipRect();
	void CreateInvenRect();
	void CreateDefualtEquip();

public:
	int GetGold();
	void GetEquipStat(STAT_INFO* _info);

public:
	HRESULT AddItem(CItem* _pItem);
	void SetSelectedItem(CItem* _pItem){ m_pSelectedItem = _pItem; m_bisItemSelected = true; }

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

public:
	static CInventoryPage* Create(CPlayer* _pPlayer, CHUD* _pHUD);

};


#endif // !__INVENTORY_PAGE_H__

