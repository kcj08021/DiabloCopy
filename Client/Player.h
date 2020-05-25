#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameObject.h"

class CColTile;
class CKeyManager;
class CAStarManager;
class CSpell;
class CHUD;
class CTerrain;

class CPlayer:
	public CGameObject{
private:
	list<const D3DXVECTOR3*> m_PathList;	//이동해야하는 타일들의 리스트
	const D3DXVECTOR3* m_pNextPath;			//목적지 타일
	CColTile* m_pColTile;					//지형맵
	CTerrain* m_pTerrain;					//실제맵

	const CGameObject* m_pTarget;			//추적하는 오브젝트

	bool m_bIsWait;							//이동 대기 여부 확인
	float m_fWaitTime;						//이동 대기시 누적 시간

	bool m_bIsAttack;						//공격확인용 변수

	bool m_bIsSpellDelay;					//마법 사용시 딜레이
	D3DXVECTOR3 m_vSpellPos;				//마법 사용 위치(마우스 좌클릭이 된 위치)

	const CSpell* m_pSpell;					//우클릭시 나가게 되는 마법
	const CSpell* m_pKeepSpell[4];			//단축키에 넣어 보관할 마법
	bool m_bIsSpellChanging;				//현재 마법 변경중인지 확인

	CHUD* m_pHUD;							//UI
	float m_fScrollXDelta;					//UI 사용시 스크롤 보정값

	bool m_bIsItemDrop;						//인벤토리에서 아이템을 버릴 준비확인
	//매니저 변수
	CKeyManager* m_pKeyMgr;					//키 매니저
	CAStarManager* m_pAstar;				//A* 매니저
	CObjectManager* m_pObjMgr;				//오브젝트 매니저

private:
	explicit CPlayer();
public:
	virtual ~CPlayer();

private:
	//이동
	void Move();
	//공격
	void Attack();
	//마법
	void Spell();
	//레벨업
	void LevelUp();

public:
	//충돌맵 설정
	void SetColTile(CColTile* _pColTile);
	void SetTerrain(CTerrain* _pTerrain){ m_pTerrain = _pTerrain; }
	void SetItemDrop(bool _bool){ m_bIsItemDrop = _bool; }
	void SetSpell(const CSpell* _pSpell){ m_pSpell = _pSpell; }
	void SetSpellHotKey(size_t _Index, const CSpell* _pSpell){ m_pKeepSpell[_Index] = _pSpell; }
	void SetIsSpellChainging(bool _bool){ m_bIsSpellChanging = _bool; }
	void AddExp(int _iExp);

public:
	CColTile* GetColTile(){ return m_pColTile; }
	const CSpell* GetSpell(){ return m_pSpell; }

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

	// 정적 멤버 함수
public:
	static CPlayer* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile, CTerrain* _pTerrain);

	friend class CStatusPage;
	friend class CHUD;
};


#endif // !__PLAYER_H__
