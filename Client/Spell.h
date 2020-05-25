#ifndef __SPELL_H__
#define __SPELL_H__

class CPlayer;
class CColTile;
class CObjectManager;

class CSpell{
protected:
	wstring m_wstrAttr;
	wstring m_wstrSpellName;
	wstring m_IconKey;
	int m_iManaRequire;
	CColTile* m_pColTile;

	const CPlayer* m_pPlayer;

	CObjectManager* m_pObjMgr;
public:
	explicit CSpell();
	virtual ~CSpell();

public:
	const wstring& GetAttribute() const { return m_wstrAttr; }
	const wstring& GetSpellName() const { return m_wstrSpellName; }
	const wstring& GetIconKey() const{ return m_IconKey; }

	int GetManaRequire() const { return m_iManaRequire; }

	void SetColTile(CColTile* _pColTile){ m_pColTile = _pColTile; }

public:
	virtual HRESULT Use(const D3DXVECTOR3 _vPos) const = 0;
};

#endif // !__SPELL_H__
