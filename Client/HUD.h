#ifndef __HUD_H__
#define __HUD_H__

#include "GameObject.h"
class CPlayer;
class CKeyManager;
class CItem;
class CPotion;
class CSpell;

class CHUD:
	public CGameObject{
public:
	enum PAGE_TYPE{ STATUS, QUEST, MAP, MENU, INVENTORY, SPELL, QUICK, BUTTON_END};
private:
	CPlayer* m_pPlayer;
	RECT m_tButtonRect[BUTTON_END];
	CGameObject* m_pPage[BUTTON_END];

	RECT m_tLevelUpButton;					//레벨업시 나타나는 버튼
	RECT m_tLevelUpText;					//레벨업시 나타나는 글자
	bool m_bIsLevelUp;						//레벨업이 되었는지 체크

	RECT m_tHpBar;
	RECT m_tMpBar;

	RECT m_tQuick[8];
	CPotion* m_pQuick[8];

	const CSpell* m_pDisplaySpell;
	vector<CSpell*> m_SpellList;
	vector<RECT*> m_pSpellRect;
	map<wstring, CSpell*> m_mapSpellHotKey;

	bool m_bIsSpellListVisible;
	
	RECT m_tInfoRect;
	wstring m_wstrInfoText;

	CKeyManager* m_pKeyMgr;
private:
	explicit CHUD();
public:
	virtual ~CHUD();

private:
	void SpellRectCreate();

public:
	void LevelUp();
	void OffLevelUpButton(){ m_bIsLevelUp = false; }
	void UseQuick(size_t _iIndex);
	HRESULT AddItem(CGameObject* _pItem);

public:
	CGameObject* GetUI(PAGE_TYPE _enum){ return m_pPage[_enum]; }
	bool GetIsLevelUpButton(RECT& _rectPos){ _rectPos = m_tLevelUpButton; return m_bIsLevelUp; }
	RECT& GetQuickRect(size_t _Index){ return m_tQuick[_Index]; }
	int GetGold();
	void GetEquipStat(STAT_INFO* _info);
	

public:
	void SetInfoText(const wstring& _wstr){ m_wstrInfoText = _wstr; }
	CPotion* SetQuickItem(size_t _Index, CPotion* _pItem);

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
	static CHUD* Create(CPlayer* _pPlayer);
};


#endif // !__HUD_H__

