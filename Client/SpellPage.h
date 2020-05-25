#ifndef __SPELL_PAGE_H__
#define __SPELL_PAGE_H__

#include "GameObject.h"

class CPlayer;
class CSpell;
class CColTile;
class CKeyManager;

class CSpellPage:
	public CGameObject{
private:
	enum{ PAGE = 4, COUNT = 7 };
	enum{ SPELL_ICON, SPELL_INFO, SPELLINFO_END };
private:
	CPlayer* m_pPlayer;

	RECT m_tPageButton[PAGE];
	RECT m_tSpellList[PAGE][COUNT][SPELLINFO_END];
	CSpell* m_pSpell[PAGE][COUNT];

	size_t m_iCurrPage;

	CKeyManager* m_pKeyMgr;
public:
	explicit CSpellPage();
	virtual ~CSpellPage();
	
private:
	void ButtonCreate();
	void SpellSetting();

public:
	void GetSpellVector(vector<CSpell*>& _vector);
	
public:
	void SetColTile(CColTile* _pColTile);

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
	static CSpellPage* Create(CPlayer* _pPlayer);

};

#endif // !__SPELL_PAGE_H__
