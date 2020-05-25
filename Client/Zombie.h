#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "GameObject.h"
class CPlayer;
class CAStarManager;

class CZombie:
	public CGameObject{
private:
	//��� ����
	list<const D3DXVECTOR3*> m_PathList;	//�̵��ؾ��ϴ� Ÿ�ϵ��� ����Ʈ
	const D3DXVECTOR3* m_pNextPath;			//������ Ÿ��
	//����Ʈ���� �ٷ� ����ؼ� �����ϴ°� �ƴ϶� ����Ʈ - �ӽ� ���� - ��ü ��ġ ���·� ����ؼ� ����

	//�������� �ٲܼ��� �ִ� ����
	CColTile* m_pColTile;					//������
	const CPlayer* m_pPlayer;				//�÷��̾�

	bool m_bIsWait;							//�̵� ��� ���� Ȯ��
	float m_fWaitTime;						//�̵� ���� ���� �ð�

	//������ ����ϱ� ���� ������
	float m_fCheckDelay;					//������ �ð� ����
	bool m_bIsDelayed;						//������ Ȯ��
	bool m_bIsAttack;						//����Ȯ�ο� ����

	bool m_bIsDieSound;

	//�Ŵ��� ����
	CAStarManager* m_pAstar;				//A*�Ŵ���

public:
	explicit CZombie();
	virtual ~CZombie();

	// �Ϲ� ��� �Լ�
private:
	//�̵�
	void Move();
	//����
	void Attack();

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
	static CZombie* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile);
};

#endif // !__ZOMBIE_H__
