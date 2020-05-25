#ifndef __STATUSE_PAGE_H__
#define __STATUSE_PAGE_H__

#include "GameObject.h"
#include "Player.h"

class CHUD;
class CKeyManager;

class CStatusPage:
	public CGameObject{
private:
	enum TEXT_BOX{ NAME, JOB, 
		LEVEL, EXP, NEXT_EXP, 
		STRENGTH_BASE,
		MAGIC_BASE,
		DEXTERITY_BASE,
		VITALITY_BASE,
		STAT_POINT,
		LIFE_BASE, LIFE_NOW,
		MANA_BASE, MANA_NOW,
		GOLD,
		ARMOR,
		TO_HIT,
		DAMAGE,
		RESIST_MAGIC,
		RESIST_FIRE,
		RESIST_LIGHTNING,
		STRENGTH_NOW,
		MAGIC_NOW,
		DEXTERITY_NOW,
		VITALITY_NOW,
		BOX_END
	};
	enum BUTTON{ UP_STRENGTH, UP_MAGIC, UP_DEXTERITY, UP_VITALITY, BUTTON_END };
private:
	CPlayer* m_pPlayer;
	CHUD* m_pHUD;
	RECT m_tStatusBox[BOX_END];
	wstring m_wstrStat[BOX_END];

	RECT m_tButton[BUTTON_END];
	int m_iButtonFrame[BUTTON_END];

	int m_iStatPoint;
	bool m_bIsButtonEnabled;

	CKeyManager* m_pKeyMgr;
private:
	explicit CStatusPage();
public:
	virtual ~CStatusPage();

private:
	void TextBoxCreate();
	void ButtonCreate();

public:
	void SetStatPoint(){ m_iStatPoint += 5; }

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
	static CStatusPage* Create(CPlayer* _pPlayer, CHUD* _pHUD);
};


#endif // !__STATUSE_PAGE_H__

