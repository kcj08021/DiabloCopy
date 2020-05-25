#ifndef __SKELETON_KING_H__
#define __SKELETON_KING_H__

#include "GameObject.h"
class CPlayer;
class CAStarManager;

class CSkeletonKing:
	public CGameObject{
	//��� ����
	list<const D3DXVECTOR3*> m_PathList;	//�̵��ؾ��ϴ� Ÿ�ϵ��� ����Ʈ
	const D3DXVECTOR3* m_pNextPath;			//������ Ÿ��
	//����Ʈ���� �ٷ� ����ؼ� �����ϴ°� �ƴ϶� ����Ʈ - �ӽ� ���� - ��ü ��ġ ���·� ����ؼ� ����

	//�������� �ٲܼ��� �ִ� ����
	CColTile* m_pColTile;					//������
	const CPlayer* m_pPlayer;				//�÷��̾�

	bool m_bIsWait;							//�̵� ��� ���� Ȯ��
	float m_fWaitTime;						//�̵� ���� ���� �ð�

	//���� ������ ����ϱ� ���� ������
	float m_fCheckDelay;					//���� ������ �ð� ����
	bool m_bIsDelayed;						//���� ������ Ȯ��

	//���̷��� ��ȯ ������
	float m_fCheckSkillDelay;
	bool m_bIsSkillDelayed;

	bool m_bIsSoundOut;
	bool m_bIsAttack;

	//�Ŵ��� ����
	CAStarManager* m_pAstar;				//A*�Ŵ���

public:
	explicit CSkeletonKing();
	virtual ~CSkeletonKing();

private:
	//�̵�
	void Move();
	//����
	void Attack();
	//��ų
	void Skill();

	// CGameObject��(��) ���� ��ӵ�
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
public:
	static CSkeletonKing* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile);
};

#endif // !__SKELETON_KING_H__
