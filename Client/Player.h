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
	list<const D3DXVECTOR3*> m_PathList;	//�̵��ؾ��ϴ� Ÿ�ϵ��� ����Ʈ
	const D3DXVECTOR3* m_pNextPath;			//������ Ÿ��
	CColTile* m_pColTile;					//������
	CTerrain* m_pTerrain;					//������

	const CGameObject* m_pTarget;			//�����ϴ� ������Ʈ

	bool m_bIsWait;							//�̵� ��� ���� Ȯ��
	float m_fWaitTime;						//�̵� ���� ���� �ð�

	bool m_bIsAttack;						//����Ȯ�ο� ����

	bool m_bIsSpellDelay;					//���� ���� ������
	D3DXVECTOR3 m_vSpellPos;				//���� ��� ��ġ(���콺 ��Ŭ���� �� ��ġ)

	const CSpell* m_pSpell;					//��Ŭ���� ������ �Ǵ� ����
	const CSpell* m_pKeepSpell[4];			//����Ű�� �־� ������ ����
	bool m_bIsSpellChanging;				//���� ���� ���������� Ȯ��

	CHUD* m_pHUD;							//UI
	float m_fScrollXDelta;					//UI ���� ��ũ�� ������

	bool m_bIsItemDrop;						//�κ��丮���� �������� ���� �غ�Ȯ��
	//�Ŵ��� ����
	CKeyManager* m_pKeyMgr;					//Ű �Ŵ���
	CAStarManager* m_pAstar;				//A* �Ŵ���
	CObjectManager* m_pObjMgr;				//������Ʈ �Ŵ���

private:
	explicit CPlayer();
public:
	virtual ~CPlayer();

private:
	//�̵�
	void Move();
	//����
	void Attack();
	//����
	void Spell();
	//������
	void LevelUp();

public:
	//�浹�� ����
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

	// CGameObject��(��) ���� ��ӵ�
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

	// ���� ��� �Լ�
public:
	static CPlayer* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile, CTerrain* _pTerrain);

	friend class CStatusPage;
	friend class CHUD;
};


#endif // !__PLAYER_H__
