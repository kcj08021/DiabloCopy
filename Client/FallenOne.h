#ifndef __FALLEN_ONE_H__
#define __FALLEN_ONE_H__

#include "GameObject.h"

class CPlayer;
class CAStarManager;

class CFallenOne:
	public CGameObject{

private:
	//경로 변수
	list<const D3DXVECTOR3*> m_PathList;	//이동해야하는 타일들의 리스트
	const D3DXVECTOR3* m_pNextPath;			//목적지 타일
	//리스트에서 바로 계산해서 전달하는게 아니라 리스트 - 임시 변수 - 객체 위치 형태로 계산해서 전달

	//옵저버로 바꿀수도 있는 변수
	CColTile* m_pColTile;					//지형맵
	const CPlayer* m_pPlayer;				//플레이어

	bool m_bIsWait;							//이동 대기 여부 확인
	float m_fWaitTime;						//이동 대기시 누적 시간

	//딜레이 계산하기 위한 변수들
	float m_fCheckDelay;					//딜레이 시간 누적
	bool m_bIsDelayed;						//딜레이 확인
	bool m_bIsAttack;						//공격확인용 변수
	bool m_bIsDieSound;
	//매니저 변수
	CAStarManager* m_pAstar;				//A*매니저

public:
	explicit CFallenOne();
	virtual ~CFallenOne();

	// 일반 멤버 함수
private:
	//이동
	void Move();
	//공격
	void Attack();

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

	static CFallenOne* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile);
};

#endif // !__FALLEN_ONE_H__
