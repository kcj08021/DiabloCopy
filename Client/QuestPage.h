#ifndef __QUEST_PAGE_H__
#define __QUEST_PAGE_H__

#include "GameObject.h"

class CPlayer;
class CKeyManager;

class CQuestPage:
	public CGameObject{

private:
	CPlayer* m_pPlayer;

	CKeyManager* m_pKeyMgr;
private:
	explicit CQuestPage();
public:
	virtual ~CQuestPage();

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
	static CQuestPage* Create(CPlayer* _pPlayer);
};

#endif // !__QUEST_PAGE_H__

