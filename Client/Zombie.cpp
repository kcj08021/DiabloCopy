#include "stdafx.h"
#include "Zombie.h"
#include "Player.h"
#include "ColTile.h"
#include "Gold.h"
#include "HPPotion.h"
#include "MPPotion.h"
#include "HeavyArmor.h"
#include "Sword.h"
#include "Ring.h"
#include "Amulet.h"
#include "Shield.h"
#include "Helmet.h"


CZombie::CZombie(): m_pColTile(nullptr), m_pPlayer(dynamic_cast<CPlayer*>(OBJMGR->GetPlayer())), m_pNextPath(nullptr),
m_fCheckDelay(0.f), m_bIsDelayed(false), m_bIsWait(false), m_fWaitTime(0.f), m_bIsDieSound(false),
m_pAstar(ASTARMGR){
}


CZombie::~CZombie(){
	Release();
}
//이동
void CZombie::Move(){
	//현재 이동 목표가 없을때
	if(nullptr != m_pNextPath){
		//목적지와 현재 객체의 거리 계산
		m_tInfo.vDir = *m_pNextPath - m_tInfo.vPos;
		float fLength = D3DXVec3Length(&m_tInfo.vDir);

		//방향 계산을 위해 정규화
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		//이동거리 계산
		D3DXVECTOR3 vSpeed = m_tInfo.vDir * 70.f * m_pTimeMgr->GetDeltaTime();

		//목적지까지의 거리가 어느정도 가까울 때
		if(fLength <= D3DXVec3Length(&vSpeed)){
			//현재 위치를 목적지로 바꿈
			m_tInfo.vPos = *m_pNextPath;
			SafeDelete(m_pNextPath);
			m_pColTile->SetPreemTile(m_tInfo.vPos, this);

			//경로 리스트가 비어있지 않다면, 새 목적지 읽어옴.
			if(!m_PathList.empty()){
				m_pNextPath = m_PathList.front();
				m_PathList.pop_front();
				//다음 이동 타일 검사
				if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
					//다음타일이 이동 불가하고, 경로 리스트가 비어있을때
					if(m_PathList.empty()){
						SafeDelete(m_pNextPath);
						m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
					} else{		//만약 다음 타일이 플레이어인 경우
						if(m_pPlayer->GetInfo().vPos == *m_pNextPath){
							//다음 타일과 경로 리스트 삭제
							SafeDelete(m_pNextPath);
							for(auto& elem : m_PathList)
								SafeDelete(elem);
							m_PathList.clear();
							//현재 위치한 타일에 도달 불가 설정
							m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
							//플레이어 방향으로 각도 전환
							D3DXVec3Normalize(&m_tInfo.vDir, &(m_pPlayer->GetInfo().vPos - m_tInfo.vPos));
							//딜레이 상태가 아닐때 공격 모션, 딜레이 상태일때는 대기 모션
							if(!m_bIsDelayed){
								if(m_wstrStateKey != L"Attack")
									m_ImgFrame = 0;		//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
								m_wstrStateKey = L"Attack";
							} else{
								m_wstrStateKey = L"Idle";
							}
						} else{
							//다음 타일이 이동이 불가한 경우 경로 재탐색
							D3DXVECTOR3 vDest(*m_PathList.back());
							if(nullptr != m_pNextPath)
								m_pAstar->PathFinding(m_PathList, *m_pNextPath, vDest, 40);
							else
								m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, vDest, 40);

							if(!m_PathList.empty()){
								SafeDelete(m_pNextPath);
								m_pNextPath = m_PathList.front();
								m_PathList.pop_front();

								//타일 선점 검사
								const CGameObject* const pPreemObj = m_pColTile->GetPreemTile(*m_pNextPath);
								if(this != pPreemObj && nullptr != pPreemObj){
									//다음 타일을 다른 객체가 선점 하고 있다면 Wait를 true로
									m_bIsWait = true;
									m_fWaitTime = 0.f;
									m_wstrStateKey = L"Idle";
									m_ImgFrame = 0;
								} else{
									//다음 타일 선점
									m_pColTile->SetPreemTile(*m_pNextPath, this);
								}
							} else{
								m_wstrStateKey = L"Idle";
								m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
							}
						}
					}
				} else{
					const CGameObject* const pPreemObj = m_pColTile->GetPreemTile(*m_pNextPath);
					if(this != pPreemObj && nullptr != pPreemObj){
						//다음 타일을 다른 객체가 선점 하고 있다면 Wait를 true로
						m_bIsWait = true;
						m_wstrStateKey = L"Idle";
						m_ImgFrame = 0;
					} else{
						//다음 타일 선점
						m_pColTile->SetPreemTile(*m_pNextPath, this);
					}
				}
			} else{
				//비어있으면 현재 위치 이동불가 상태로 전환
				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
			}
		} else{
			//가깝지 않다면 목적지까지 이동하게 함
			m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);
			m_tInfo.vPos += vSpeed;
			m_pColTile->SetPreemTile(m_tInfo.vPos, this);
		}
	} else{
		//이동 목표가 없으므로 대기 상태로 전환
		m_wstrStateKey = L"Idle";
		m_pColTile->SetPreemTile(m_tInfo.vPos, this);
		m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
	}

	CGameObject::Animation();
}
//공격
void CZombie::Attack(){
	//현재 공격 모션 종료시 딜레이 시작
	if(m_bIsImgEnd){
		m_bIsDelayed = true;
		m_bIsAttack = false;

	} else if(m_ImgCount - 3 == CGameObject::Animation() && !m_bIsAttack){
		size_t index = m_pColTile->GetIndex(m_tInfo.vPos);
		//각도에 따른 인덱스 계산
		if(m_wstrDirectionKey == L"Down")
			index += (TILEX << 1) + 1;
		else if(m_wstrDirectionKey == L"LeftDown")
			index += (TILEX << 1);
		else if(m_wstrDirectionKey == L"LeftUp")
			index += -1;
		else if(m_wstrDirectionKey == L"Left")
			index += (TILEX << 1) - 1;
		else if(m_wstrDirectionKey == L"Right")
			index += -(TILEX << 1) + 1;
		else if(m_wstrDirectionKey == L"RightDown")
			index += 1;
		else if(m_wstrDirectionKey == L"RightUp")
			index += -(TILEX << 1);
		else if(m_wstrDirectionKey == L"Up")
			index += -(TILEX << 1) - 1;
		//타일에 데미지 설정
		m_pColTile->SetDamageTile(index, 10);
		m_bIsAttack = true;

	}
}

HRESULT CZombie::Release(){
	for(auto& elem : m_PathList)
		SafeDelete(elem);
	m_PathList.clear();

	SafeDelete(m_pNextPath);

	return S_OK;
}

HRESULT CZombie::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Monster";
	m_wstrObjectKey = L"Zombie";
	m_wstrStateKey = L"Idle";
	m_wstrDirectionKey = L"Down";

	m_tStat.szName = L"ZOMBIE";
	m_tStat.szJob = L"";

	m_tStat.iLevel = 1;
	m_tStat.iExp = 20;
	m_tStat.iNextExp = 0;

	m_tStat.iStrength = 0;
	m_tStat.iDexterity = 0;
	m_tStat.iMagic = 0;
	m_tStat.iVitality = 0;

	m_tStat.iMaxHp = 30;
	m_tStat.iHp = 30;
	m_tStat.iMaxMp = 0;
	m_tStat.iMp = 0;

	m_ptRectSize.x = 64;
	m_ptRectSize.y = 120;

	CGameObject::UpdateRect();

	return S_OK;
}

HRESULT CZombie::LateInit(){
	m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
	m_pColTile->SetPreemTile(m_tInfo.vPos, this);

	return S_OK;
}

HRESULT CZombie::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	if(!m_bIsDead){
		//스프라이트 처리 위한 텍스쳐 갯수 정보 얻어옴
		const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
		m_ImgCount = pTextureInfo->iImageCount;
		//죽음 상태가 아닐때
		if(m_wstrStateKey != L"Die"){
			//플레이어와의 거리계산
			float fDistance = D3DXVec3Length(&(m_tInfo.vPos - m_pPlayer->GetInfo().vPos));
			if(400.f > fDistance && (m_PathList.empty() || *m_PathList.back() != m_pColTile->GetTile(m_pPlayer->GetInfo().vPos)->vPos)){
				if(nullptr == m_pNextPath)
					m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, m_pPlayer->GetInfo().vPos, 40);
				else
					m_pAstar->PathFinding(m_PathList, *m_pNextPath, m_pPlayer->GetInfo().vPos, 40);

				//길찾기 경로가 있으면 걷기 모션으로 전환
				if(!m_PathList.empty()){
					m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);
					m_pColTile->SetPreemTile(m_tInfo.vPos, this);
					//현재 이동중인 목적지가 비어있을때 새로 목적지 설정
					if(nullptr == m_pNextPath){
						m_pNextPath = m_PathList.front();
						m_PathList.pop_front();
					}

					//다음 이동 타일 검사
					const CGameObject* const pPreemObj = m_pColTile->GetPreemTile(*m_pNextPath);
					if(this != pPreemObj && nullptr != pPreemObj){
						//다음 타일을 다른 객체가 선점 하고 있다면 Wait를 true로
						m_bIsWait = true;
						m_fWaitTime = 0.f;
						m_wstrStateKey = L"Idle";
						m_ImgFrame = 0;
					} else{
						//다음 타일 선점
						m_pColTile->SetPreemTile(*m_pNextPath, this);

						if(m_wstrStateKey != L"Walk")
							m_ImgFrame = 0;		//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
						m_wstrStateKey = L"Walk";
					}
				} else{		//길찾기 경로가 없을때
					m_pColTile->SetPreemTile(m_tInfo.vPos, this);
					if(nullptr != m_pNextPath){		//이동해야하는 경로가 있을때
						const CGameObject* const pPreemObj = m_pColTile->GetPreemTile(*m_pNextPath);
						if(this != pPreemObj && nullptr != pPreemObj){
							//다음 타일을 다른 객체가 선점 하고 있다면 Wait를 true로
							m_bIsWait = true;
							m_fWaitTime = 0.f;
							m_wstrStateKey = L"Idle";
							m_ImgFrame = 0;
						} else{
							//다음 타일 선점
							m_pColTile->SetPreemTile(*m_pNextPath, this);
						}
					} else{
						m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);

						//방향을 플레이어 방향으로 전환
						D3DXVec3Normalize(&m_tInfo.vDir, &(m_pPlayer->GetInfo().vPos - m_tInfo.vPos));
						//딜레이 상태가 아닐때 공격 모션, 딜레이 상태일때는 대기 모션
						if(!m_bIsDelayed){
							if(m_wstrStateKey != L"Attack")
								m_ImgFrame = 0;		//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
							m_wstrStateKey = L"Attack";
						} else{
							m_wstrStateKey = L"Idle";
						}
					}
				}
			}
		}

		//현재 모션에 따른 행동 전환
		if(m_wstrStateKey == L"Walk"){
			Move();
		} else if(m_wstrStateKey == L"Attack"){
			if(!m_bIsDelayed)
				Attack();
		} else if(m_wstrStateKey == L"Die"){
			if(m_wstrStateKey == L"Die" && !m_bIsDieSound){
				m_pSoundMgr->PlaySound(L"Zombie.wav", CSoundManager::MONSTER);
				m_bIsDieSound = true;
			}
			CGameObject::Animation();
			if(m_bIsImgEnd){
				m_ImgFrame = m_ImgCount - 1;
				m_bIsDead = true;
				const_cast<CPlayer*>(m_pPlayer)->AddExp(m_tStat.iExp);
				CGameObject* pObj = nullptr;
				switch(rand() % 10){
				case 0:
				case 1:
					pObj = CGold::Create(m_tInfo.vPos, rand() % 100 + 1);
					if(nullptr != pObj)
						OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
					break;
				case 2:
					pObj = CHPPotion::Create(m_tInfo.vPos, const_cast<CPlayer*>(m_pPlayer));
					if(nullptr != pObj)
						OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
					break;
				case 3:
					pObj = CMPPotion::Create(m_tInfo.vPos, const_cast<CPlayer*>(m_pPlayer));
					if(nullptr != pObj)
						OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
					break;
				case 4:
					switch(rand() % 6){
					case 0:
						pObj = CSword::Create(m_tInfo.vPos);
						if(nullptr != pObj)
							OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
						break;
					case 1:
						pObj = CShield::Create(m_tInfo.vPos);
						if(nullptr != pObj)
							OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
						break;
					case 2:
						pObj = CHeavyArmor::Create(m_tInfo.vPos);
						if(nullptr != pObj)
							OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
						break;
					case 3:
						pObj = CRing::Create(m_tInfo.vPos);
						if(nullptr != pObj)
							OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
						break;
					case 4:
						pObj = CHelmet::Create(m_tInfo.vPos);
						if(nullptr != pObj)
							OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
						break;
					case 5:
						pObj = CAmulet::Create(m_tInfo.vPos);
						if(nullptr != pObj)
							OBJMGR->AddObject(CObjectManager::OBJ_ITEM, pObj);
						break;
					default:
						break;
					}
				default:
					break;
				}

				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);		//이동 불가 해제
				m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);					//타일 선점 해제
				if(nullptr != m_pNextPath)
					m_pColTile->SetPreemTile(*m_pNextPath, nullptr);				//다음 타일 선점해제

				SafeDelete(m_pNextPath);											//이동 타일 할당 해제
				for(auto& elem : m_PathList)										//경로리스트 할당 해제
					SafeDelete(elem);
				m_PathList.clear();
			}
		} else{
			CGameObject::Animation();
			if(m_bIsDelayed){		//딜레이 상태인 경우
									//딜레이 시간 계산
				m_fCheckDelay += m_pTimeMgr->GetDeltaTime();
				//딜레이 시간이 넘어가면 딜레이 상태 해제
				if(m_fCheckDelay > 1.f){
					m_bIsDelayed = false;
					m_fCheckDelay = 0.f;
					m_ImgFrame = 0;			//딜레이 상태 해제시 스프라이트 프레임 0 초기화
				}
			} else if(m_bIsWait){
				//이동중 타일 선점 실패하여 대기중 일시
				if(nullptr != m_pNextPath){
					//찾은 경로 모두 삭제(다음 Update()에서 경로를 다시 찾을 수 있도록)
					if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
						SafeDelete(m_pNextPath);
						if(!m_PathList.empty()){
							for(auto& elem : m_PathList)
								SafeDelete(elem);
							m_PathList.clear();
						}
						m_pColTile->SetPreemTile(m_tInfo.vPos, this);
						m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
					} else{
						//이동이 가능하다면 현재 다른 객체가 선점하고 있는지 체크
						m_pColTile->SetPreemTile(m_tInfo.vPos, this);
						const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
						if(nullptr == pObj){
							m_pColTile->SetPreemTile(*m_pNextPath, this);
							m_bIsWait = false;
							m_ImgFrame = 0;
							m_wstrStateKey = L"Walk";
							//대기상태가 너무 길어진다면 경로 삭제뒤, 정지상태로 전환(다른 객체들과 정면으로 충돌시 빠져나오게 함)
						} else if(m_fWaitTime > 1.f){
							m_fWaitTime = 0.f;
							m_bIsWait = false;
							SafeDelete(m_pNextPath);
							if(!m_PathList.empty()){
								for(auto& elem : m_PathList)
									SafeDelete(elem);
								m_PathList.clear();
							}
							m_pColTile->SetPreemTile(m_tInfo.vPos, this);
							m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
						}
					}
				}
			}
		}
	}

	CGameObject::UpdateAngle();
	CGameObject::UpdateRect();

	return S_OK;
}


HRESULT CZombie::LateUpdate(){
	if(m_wstrStateKey != L"Die"){
		int iDamage = 0;
		if(iDamage = m_pColTile->GetDamage(m_tInfo.vPos)){
			SetDamage(iDamage);
			m_pSoundMgr->PlaySound(L"MonsterHit.wav", CSoundManager::MONSTER);

		}
	}

	//이동중 타일 선점 실패하여 대기중 일시
	if(m_bIsWait){
		if(nullptr != m_pNextPath){
			//다음에 이동해야 하는 타일이 현재 이동 불가가 되었다면
			if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
				//찾은 경로 모두 삭제(다음 Update()에서 경로를 다시 찾을 수 있도록)
				SafeDelete(m_pNextPath);
				if(!m_PathList.empty()){
					for(auto& elem : m_PathList)
						SafeDelete(elem);
					m_PathList.clear();
				}
				m_pColTile->SetPreemTile(m_tInfo.vPos, this);
				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
			} else{
				//이동이 가능하다면 현재 다른 객체가 선점하고 있는지 체크
				m_fWaitTime += m_pTimeMgr->GetDeltaTime();		//대기 시간 누적(LateUpdate()에서 한번만 누적함(Update()에서도 누적시 이중으로 누적됨)
				const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
				if(nullptr == pObj){
					m_pColTile->SetPreemTile(*m_pNextPath, this);
					m_bIsWait = false;
					m_ImgFrame = 0;
					m_wstrStateKey = L"Walk";
					//대기상태가 너무 길어진다면 경로 삭제뒤, 정지상태로 전환(다른 객체들과 정면으로 충돌시 빠져나오게 함)
				} else if(m_fWaitTime > 1.f){
					m_fWaitTime = 0.f;
					m_bIsWait = false;
					SafeDelete(m_pNextPath);
					if(!m_PathList.empty()){
						for(auto& elem : m_PathList)
							SafeDelete(elem);
						m_PathList.clear();
					}
					m_pColTile->SetPreemTile(m_tInfo.vPos, this);
					m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
				}
			}
		}
	}

	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

	if((vScreen.x < -TILECX*2.f || vScreen.x >= WINCX + TILECX*2.f || vScreen.y <= -TILECY*2.f || vScreen.y >= 700 + TILECY*2.f))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;

	return S_OK;
}

HRESULT CZombie::Render(){
	if(m_bIsVisible){
		const TEXTURE_INFO* pTextureInfo = nullptr;

		D3DXMATRIX matScale, matTrans, matWorld;

		const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, vScreen.x, vScreen.y, 0.f);
		matWorld = matScale * matTrans;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);

		if(nullptr != pTextureInfo){
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pDeviceMgr->GetSprite()->End();
		}
		if(!m_bIsDead)
			if(KEYMGR->IsKeyPressing(KEY_D)){
				D3DXVECTOR2 pt[5] = { { static_cast<float>(m_tRect.left), static_cast<float>(m_tRect.top) },
				{ static_cast<float>(m_tRect.right), static_cast<float>(m_tRect.top) },
				{ static_cast<float>(m_tRect.right), static_cast<float>(m_tRect.bottom) },
				{ static_cast<float>(m_tRect.left), static_cast<float>(m_tRect.bottom) },
				{ static_cast<float>(m_tRect.left), static_cast<float>(m_tRect.top) }
				};

				m_pDeviceMgr->GetLine()->Begin();
				m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_pDeviceMgr->GetLine()->End();
			}
	}
	return S_OK;
}

CZombie* CZombie::Create(const D3DXVECTOR3 & _vPos, CColTile* _pColTile){
	CZombie* pInstance = new CZombie;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_pColTile = _pColTile;

	return pInstance;
}
