#include "stdafx.h"
#include "Player.h"
#include "ColTile.h"
#include "Terrain.h"
#include "Spell.h"
#include "HUD.h"
#include "SpellPage.h"
#include "InventoryPage.h"
#include "Item.h"
#include "MapObject.h"

CPlayer::CPlayer(): m_pColTile(nullptr), m_pNextPath(nullptr), m_pTarget(nullptr), m_bIsWait(false),
m_bIsSpellDelay(false), m_pSpell(nullptr), m_bIsSpellChanging(false), m_fWaitTime(0.f),
m_pHUD(nullptr), m_fScrollXDelta(0.f),
m_bIsItemDrop(false),
m_pKeyMgr(KEYMGR), m_pAstar(ASTARMGR), m_pObjMgr(OBJMGR){
	INIT_STRUCT(m_vSpellPos);

	INIT_ARRAY(m_pKeepSpell);
}


CPlayer::~CPlayer(){
	Release();
}
//이동
void CPlayer::Move(){
	//현재 이동 목표가 없을때
	if(nullptr != m_pNextPath){
		//목적지와 현재 객체의 거리 계산
		m_tInfo.vDir = *m_pNextPath - m_tInfo.vPos;
		float fLength = D3DXVec3Length(&m_tInfo.vDir);

		//방향 계산을 위해 정규화
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		//이동거리 계산
		D3DXVECTOR3 vSpeed = m_tInfo.vDir * 200.f * m_pTimeMgr->GetDeltaTime();

		//목적지까지의 거리가 이동 거리보다 짧을 때
		if(fLength <= D3DXVec3Length(&vSpeed)){
			//현재 위치를 목적지로 바꿈
			m_tInfo.vPos = *m_pNextPath;
			m_pScrollMgr->SetScroll(m_pNextPath->x - (WINCX >> 1) + m_fScrollXDelta, m_pNextPath->y - (WINCY - 256)*0.5f);
			SafeDelete(m_pNextPath);

			if(!m_PathList.empty()){
				/*여기에서 다음 타일 검사 해야함 아니면 LateUpdate()나*/
				m_pNextPath = m_PathList.front();
				m_PathList.pop_front();
				//이동해야하는 다음타일 검사
				if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
					//다음 타일이 이동이 불가한 경우 경로 재탐색
					D3DXVECTOR3 vDest(*m_PathList.back());
					m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, vDest);
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
				if(m_pNextPath == nullptr)
					m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
			}
		} else{
			//가깝지 않다면 목적지까지 이동하게 함
			m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);
			m_tInfo.vPos += vSpeed;
			m_pColTile->SetPreemTile(m_tInfo.vPos, this);

			//플레이어 캐릭터 이동시 스크롤에도 이동값 넣어줌
			m_pScrollMgr->MoveScroll(vSpeed.x, vSpeed.y);
		}
	} else{
		if(nullptr != m_pTarget){
			//현재 타겟과의 각도 계산
			if(m_pTarget->GetTypeKey() == L"Monster" || m_pTarget->GetTypeKey() == L"Boss"){
				D3DXVec3Normalize(&m_tInfo.vDir, &(m_pTarget->GetInfo().vPos - m_tInfo.vPos));
				if(m_wstrStateKey != L"Attack"){
					m_pSoundMgr->PlaySound(L"Attack.wav", CSoundManager::PLAYER);
					m_ImgFrame = 0;			//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
				}
				m_wstrStateKey = L"Attack";
			} else if(m_pTarget->GetTypeKey() == L"Item"){
				auto& ItemList = m_pObjMgr->GetList(CObjectManager::OBJ_ITEM);
				list<CGameObject*>::iterator iter = ItemList.begin();
				for(; iter != ItemList.end(); ++iter)
					if(*iter == m_pTarget){
						if(m_pHUD->GetUI(CHUD::INVENTORY)->IsVisible()){
							dynamic_cast<CInventoryPage*>(m_pHUD->GetUI(CHUD::INVENTORY))->SetSelectedItem(dynamic_cast<CItem*>(*iter));
							ItemList.erase(iter);
							m_pTarget = nullptr;
						} else{
							if(!FAILED(m_pHUD->AddItem(*iter))){
								ItemList.erase(iter);
								m_pTarget = nullptr;
							} else{
								m_pSoundMgr->PlaySound(L"InvenFull.wav", CSoundManager::PLAYER);
							}
						}
						break;
					}

				if(m_wstrStateKey != L"Idle")
					m_ImgFrame = 0;			//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
				m_wstrStateKey = L"Idle";
			} else if(m_pTarget->GetTypeKey() == L"Object"){
				for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_MAPOBJ)){
					if(elem == m_pTarget){
						dynamic_cast<CMapObject*>(elem)->Activate();
						break;
					}
				}

				if(m_wstrStateKey != L"Idle")
					m_ImgFrame = 0;			//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
				m_wstrStateKey = L"Idle";
			}
		} else
			//이동 목표가 없으므로 대기 상태로 전환
			m_wstrStateKey = L"Idle";
	}

	CGameObject::Animation();
}
//공격
void CPlayer::Attack(){
	int iFrame = CGameObject::Animation(0.033f);
	STAT_INFO tEquip;
	m_pHUD->GetEquipStat(&tEquip);
	if(m_ImgCount - 7 == iFrame && !m_bIsAttack){
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
		m_pColTile->SetDamageTile(index, m_tStat.iDamage + tEquip.iDamage);
		m_bIsAttack = true;
	} else if(m_bIsImgEnd){
		m_bIsAttack = false;
		//좌클릭을 유지하고 있다면 계속 공격 상태 유지, 아니면 대기 상태로 전환
		if(!m_pKeyMgr->IsKeyPressing(KEY_LBUTTON)){
			m_ImgFrame = 0;
			m_wstrStateKey = L"Idle";
		} else{
			m_pSoundMgr->PlaySound(L"Attack.wav", CSoundManager::PLAYER);
		}
	}
}

void CPlayer::Spell(){
	int iFrame = CGameObject::Animation(0.033f);
	if(m_wstrStateKey == L"Magic"){
		m_wstrStateKey += m_pSpell->GetAttribute();
		if(m_wstrStateKey == L"MagicFire")
			m_pSoundMgr->PlaySound(L"MagicFire.wav", CSoundManager::EFFECT);
		else if(m_wstrStateKey == L"MagicLight")
			m_pSoundMgr->PlaySound(L"MagicLight.wav", CSoundManager::EFFECT);
		else if(m_wstrStateKey == L"MagicNone")
			m_pSoundMgr->PlaySound(L"MagicNone.wav", CSoundManager::EFFECT);

	}
	if(m_ImgCount - 6 == iFrame){
		if(!m_bIsSpellDelay){
			m_bIsSpellDelay = true;
			m_pSpell->Use(m_vSpellPos);
			m_tStat.iMp -= m_pSpell->GetManaRequire();
			INIT_STRUCT(m_vSpellPos);
		}
	} else if(m_bIsImgEnd){
		m_ImgFrame = 0;
		m_bIsSpellDelay = false;
		m_wstrStateKey = L"Idle";
	}
}

void CPlayer::LevelUp(){
	++m_tStat.iLevel;
	m_tStat.iMaxHp += 2;
	m_tStat.iMaxMp += 1;

	m_pHUD->LevelUp();
}


void CPlayer::AddExp(int _iExp){
	if(m_wstrStateKey != L"Die")
		m_tStat.iExp += _iExp;

	if(m_tStat.iNextExp != 0 && m_tStat.iExp >= m_tStat.iNextExp){
		LevelUp();
		m_tStat.iNextExp *= 2;
		CPlayer::AddExp(0);
	}
}

void CPlayer::SetColTile(CColTile* _pColTile){
	m_pColTile = _pColTile; 
	dynamic_cast<CSpellPage*>(m_pHUD->GetUI(CHUD::SPELL))->SetColTile(_pColTile);
}

HRESULT CPlayer::Release(){
	for(auto& elem : m_PathList)
		SafeDelete(elem);
	m_PathList.clear();

	SafeDelete(m_pNextPath);

	return S_OK;
}

HRESULT CPlayer::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Warrior";
	m_wstrObjectKey = L"SwordShield";
	m_wstrStateKey = L"Idle";
	m_wstrDirectionKey = L"Down";

	//플레이어 정보 구조체
	m_tStat.szName = L"JUSIN";
	m_tStat.szJob = L"WARRIOR";

	m_tStat.iLevel = 1;
	m_tStat.iExp = 0;
	m_tStat.iNextExp = 10;

	m_tStat.iStrength = 30;
	m_tStat.iDexterity = 10;
	m_tStat.iMagic = 20;
	m_tStat.iVitality = 25;

	m_tStat.iDamage = 10 + (m_tStat.iStrength * m_tStat.iLevel) / 100;

	m_tStat.iMaxHp = 100+ m_tStat.iVitality;
	m_tStat.iHp = 100 + m_tStat.iVitality;
	m_tStat.iMaxMp =100 + m_tStat.iMagic;
	m_tStat.iMp =100+ m_tStat.iMagic;

	m_ptRectSize.x = 64;
	m_ptRectSize.y = 120;

	CGameObject::UpdateRect();

	return S_OK;
}

HRESULT CPlayer::LateInit(){
	m_pHUD = CHUD::Create(this);
	m_pHUD->Update();

	if(nullptr != m_pHUD)
		m_pObjMgr->AddObject(CObjectManager::OBJ_UI, m_pHUD);

	m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);

	return S_OK;
}

HRESULT CPlayer::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	if(!m_bIsDead){
		//스프라이트 처리 위한 텍스쳐 갯수 정보 얻어옴
		const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
		m_ImgCount = pTextureInfo->iImageCount;

		//캐릭터 조작 부분
		/////////////////////////////////////////////////////////////////////////////
		D3DXVECTOR3 vMousePos = m_pKeyMgr->GetMousePos();		//마우스 좌표

		wstring wstrInfo = L"";
		if((vMousePos.x > (m_fScrollXDelta < 0 ? 640 : 0) && vMousePos.x < (m_fScrollXDelta > 0 ? WINCX - 640 : WINCX)) &&
			(vMousePos.y > 0 && vMousePos.y < (WINCY - 256))){
			for(auto& elem : m_pObjMgr->GetList(CObjectManager::OBJ_MONSTER)){
				if(elem->IsVisible() && !elem->GetIsDead())
					if(PtInRect(&elem->GetRect(), POINT{ static_cast<LONG>(vMousePos.x),static_cast<LONG>(vMousePos.y) })){
						wstrInfo = elem->GetStat().szName;
						break;
					}
			}
			if(wstrInfo == L"")
				for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_ITEM)){
					if(PtInRect(&elem->GetRect(), POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
						wstrInfo = elem->GetStat().szName;
						break;
					}
				}

			if(wstrInfo == L"")
				for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_MAPOBJ)){
					if(PtInRect(&elem->GetRect(), POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
						wstrInfo = elem->GetStat().szName;
						break;
					}
				}
		}

		m_pHUD->SetInfoText(wstrInfo);
		if((vMousePos.x > (m_fScrollXDelta < 0 ? 640 : 0) && vMousePos.x < (m_fScrollXDelta > 0 ? WINCX - 640 : WINCX)) &&
			(vMousePos.y > 0 && vMousePos.y < (WINCY - 256)) && !m_bIsItemDrop && !m_bIsSpellChanging){
			if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
				RECT rc = {};
				BOOL bIsLevelUpClicked = false;
				if(m_pHUD->GetIsLevelUpButton(rc))
					if(bIsLevelUpClicked = PtInRect(&rc, POINT{ static_cast<LONG>(vMousePos.x),static_cast<LONG>(vMousePos.y) })){
						m_pHUD->GetUI(CHUD::STATUS)->SetVisible(true);
						m_pHUD->OffLevelUpButton();
					}

				if(!m_bIsSpellDelay){
					//좌클릭시 현재 목표로 설정된 오브젝트 포인터를 비움
					m_pTarget = nullptr;

					//쉬프트 누른 상태에서 좌클릭시
					if(m_pKeyMgr->IsKeyPressing(KEY_SHIFT)){
						//경로 리스트내 경로 삭제 및 초기화
						for(auto& elem : m_PathList)
							SafeDelete(elem);
						m_PathList.clear();

						//마우스 클릭 지점과 현재 객체 위치간 방향 계산
						D3DXVec3Normalize(&m_tInfo.vDir, &(m_pKeyMgr->GetMousePos() - (m_tInfo.vPos - m_pScrollMgr->GetScroll())));

						//현재 이동중인 목적지가 없다면 공격 상태로 전환
						if(nullptr == m_pNextPath){
							if(m_wstrStateKey != L"Attack"){
								m_ImgFrame = 0;			//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
								m_pSoundMgr->PlaySound(L"Attack.wav", CSoundManager::PLAYER);
							}
							m_wstrStateKey = L"Attack";
						}
						/////////////////////////////////////////////////////////////////////////////
					} else{
						//그냥 좌클릭시 클릭
						if(!bIsLevelUpClicked){
							//클릭한 지점에 오브젝트가 있는지 판별, 있다면 목표로 설정
							for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_MONSTER)){
								if(!elem->GetIsDead())
									if(PtInRect(&elem->GetRect(), POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
										m_pTarget = elem;
										break;
									}
							}

							if(nullptr == m_pTarget)
								for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_ITEM)){
									if(PtInRect(&elem->GetRect(), POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
										m_pTarget = elem;
										break;
									}
								}

							if(nullptr == m_pTarget)
								for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_MAPOBJ)){
									if(!elem->GetIsDead())
										if(PtInRect(&elem->GetRect(), POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
											m_pTarget = elem;
											break;
										}
								}


							//해당 지점으로 길찾기
							//현재 위치 이동불가 해제
							m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);
							if(nullptr == m_pTarget){
								if(nullptr != m_pNextPath)
									m_pAstar->PathFinding(m_PathList, *m_pNextPath, vMousePos + m_pScrollMgr->GetScroll());
								else
									m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, vMousePos + m_pScrollMgr->GetScroll());
							} else{
								//목표가 있을땐 목표 바로 앞 지점까지 경로로 설정
								if(nullptr != m_pNextPath)
									m_pAstar->PathFinding(m_PathList, *m_pNextPath, m_pTarget->GetInfo().vPos);
								else
									m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
							}
							//경로 리스트가 비어있지 않을때
							if(!m_PathList.empty()){
								//현재 이동중인 목적지가 비어있을때 새로 목적지 설정

								if(nullptr == m_pNextPath){
									m_pNextPath = m_PathList.front();
									m_PathList.pop_front();

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
								}
								if(!m_bIsWait){
									if(m_wstrStateKey != L"Walk")
										m_ImgFrame = 0;			//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
									m_wstrStateKey = L"Walk";
								}
							} else{
								if(m_pNextPath == nullptr)
									m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
								if(nullptr != m_pTarget){
									if(m_pTarget->GetTypeKey() == L"Monster" || m_pTarget->GetTypeKey() == L"Boss"){ 	//해당 타겟이 몬스터인 경우
										D3DXVec3Normalize(&m_tInfo.vDir, &(m_pTarget->GetInfo().vPos - m_tInfo.vPos));
										if(m_wstrStateKey != L"Attack"){
											m_ImgFrame = 0;			//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
											m_pSoundMgr->PlaySound(L"Attack.wav", CSoundManager::PLAYER);
										}
										m_wstrStateKey = L"Attack";
									} else if(m_pTarget->GetTypeKey() == L"Item"){ //해당 타겟이 아이템인 경우
										auto& ItemList = m_pObjMgr->GetList(CObjectManager::OBJ_ITEM);
										list<CGameObject*>::iterator iter = ItemList.begin();
										for(; iter != ItemList.end(); ++iter)
											if(*iter == m_pTarget){
												if(m_pHUD->GetUI(CHUD::INVENTORY)->IsVisible()){
													dynamic_cast<CInventoryPage*>(m_pHUD->GetUI(CHUD::INVENTORY))->SetSelectedItem(dynamic_cast<CItem*>(*iter));
													ItemList.erase(iter);
													m_pTarget = nullptr;
												} else{
													if(!FAILED(m_pHUD->AddItem(*iter))){
														ItemList.erase(iter);
														m_pTarget = nullptr;
													} else{
														m_pSoundMgr->PlaySound(L"InvenFull.wav", CSoundManager::PLAYER);
													}
												}
												break;
											}
										if(m_wstrStateKey != L"Idle")
											m_ImgFrame = 0;			//이전 상태와 현재 상태가 다른 경우 스프라이트 프레임 0 초기화
										m_wstrStateKey = L"Idle";
									}
								}
							}
						}
					}
				}
				/////////////////////////////////////////////////////////////////////////////
				//우클릭시 마법 사용
		} else if(m_pKeyMgr->IsKeyDown(KEY_RBUTTON)){
			if((vMousePos.x > (m_fScrollXDelta < 0 ? 640 : 0) && vMousePos.x < (m_fScrollXDelta > 0 ? WINCX - 640 : WINCX)) &&
				(vMousePos.y > 0 && vMousePos.y < (WINCY - 256))){
				for(auto& elem : m_PathList)
					SafeDelete(elem);
				m_PathList.clear();
				if(nullptr == m_pNextPath){
					if(nullptr != m_pSpell){
						m_vSpellPos = m_pKeyMgr->GetMousePos();
						D3DXVec3Normalize(&m_tInfo.vDir, &(m_vSpellPos - (m_tInfo.vPos - m_pScrollMgr->GetScroll())));
						if(L"Idle" == m_wstrStateKey && m_pNextPath == nullptr){
							m_ImgFrame = 0;
							m_wstrStateKey = L"Magic";
						}
					}
				}
			}
		}
	}

		if(!m_bIsSpellChanging && m_pKeyMgr->IsKeyDown(KEY_F5)){
			m_pSpell = m_pKeepSpell[0];
		} else if(!m_bIsSpellChanging && m_pKeyMgr->IsKeyDown(KEY_F6)){
			m_pSpell = m_pKeepSpell[1];
		} else if(!m_bIsSpellChanging && m_pKeyMgr->IsKeyDown(KEY_F7)){
			m_pSpell = m_pKeepSpell[2];
		} else if(!m_bIsSpellChanging && m_pKeyMgr->IsKeyDown(KEY_F8)){
			m_pSpell = m_pKeepSpell[3];
		} else if(m_pKeyMgr->IsKeyDown(KEY_C)){
			m_pHUD->GetUI(CHUD::STATUS)->SetVisible(!m_pHUD->GetUI(CHUD::STATUS)->IsVisible());
			m_pHUD->GetUI(CHUD::QUEST)->SetVisible(false);
		} else if(m_pKeyMgr->IsKeyDown(KEY_Q)){
			m_pHUD->GetUI(CHUD::QUEST)->SetVisible(!m_pHUD->GetUI(CHUD::QUEST)->IsVisible());
			m_pHUD->GetUI(CHUD::STATUS)->SetVisible(false);
		} else if(m_pKeyMgr->IsKeyDown(KEY_I)){
			m_pHUD->GetUI(CHUD::INVENTORY)->SetVisible(!m_pHUD->GetUI(CHUD::INVENTORY)->IsVisible());
			m_pHUD->GetUI(CHUD::SPELL)->SetVisible(false);
		} else if(m_pKeyMgr->IsKeyDown(KEY_B)){
			m_pHUD->GetUI(CHUD::SPELL)->SetVisible(!m_pHUD->GetUI(CHUD::SPELL)->IsVisible());
			m_pHUD->GetUI(CHUD::INVENTORY)->SetVisible(false);
		} else if(m_pKeyMgr->IsKeyDown(KEY_1)){
			m_pHUD->UseQuick(0);
		} else if(m_pKeyMgr->IsKeyDown(KEY_2)){
			m_pHUD->UseQuick(1);
		} else if(m_pKeyMgr->IsKeyDown(KEY_3)){
			m_pHUD->UseQuick(2);
		} else if(m_pKeyMgr->IsKeyDown(KEY_4)){
			m_pHUD->UseQuick(3);
		} else if(m_pKeyMgr->IsKeyDown(KEY_5)){
			m_pHUD->UseQuick(4);
		} else if(m_pKeyMgr->IsKeyDown(KEY_6)){
			m_pHUD->UseQuick(5);
		} else if(m_pKeyMgr->IsKeyDown(KEY_7)){
			m_pHUD->UseQuick(6);
		} else if(m_pKeyMgr->IsKeyDown(KEY_8)){
			m_pHUD->UseQuick(7);
		}else if(m_pKeyMgr->IsKeyDown(KEY_TAB)){
			m_pTerrain->SetMiniMap();
		}

		if((m_pHUD->GetUI(CHUD::QUEST)->IsVisible() || m_pHUD->GetUI(CHUD::STATUS)->IsVisible()) &&
			!(m_pHUD->GetUI(CHUD::SPELL)->IsVisible() || m_pHUD->GetUI(CHUD::INVENTORY)->IsVisible()))
			m_fScrollXDelta = -320.f;
		else if(!(m_pHUD->GetUI(CHUD::QUEST)->IsVisible() || m_pHUD->GetUI(CHUD::STATUS)->IsVisible()) &&
			(m_pHUD->GetUI(CHUD::SPELL)->IsVisible() || m_pHUD->GetUI(CHUD::INVENTORY)->IsVisible()))
			m_fScrollXDelta = 320.f;
		else
			m_fScrollXDelta = 0.f;

		m_pScrollMgr->SetScroll(m_tInfo.vPos.x - WINCX*0.5f + m_fScrollXDelta, m_tInfo.vPos.y - (WINCY - 256)*0.5f);


		//현재 모션에 따른 행동 함수 작동
		if(m_wstrStateKey == L"Walk"){
			Move();
		} else if(m_wstrStateKey == L"Attack"){
			Attack();
		} else if(m_wstrStateKey.substr(0, 5) == L"Magic"){
			Spell();
		} else if(m_wstrStateKey == L"Hit"){
			CGameObject::Animation();
			if(m_bIsImgEnd){
				m_ImgFrame = 0;
				m_wstrStateKey = L"Idle";
			}
		} else if(m_wstrStateKey == L"Die"){
			CGameObject::Animation(0.1f);
			if(m_bIsImgEnd){
				m_ImgFrame = m_ImgCount - 1;
				m_bIsDead = true;

				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);		//이동 불가 해제
				m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);					//타일 선점 해제
				if(nullptr != m_pNextPath)
					m_pColTile->SetPreemTile(*m_pNextPath, nullptr);				//다음 타일 선점해제

				SafeDelete(m_pNextPath);											//이동 타일 할당 해제
				for(auto& elem : m_PathList)										//경로리스트 할당 해제
					SafeDelete(elem);
				m_PathList.clear();
			}
		} else if(m_wstrStateKey == L"Block"){
			CGameObject::Animation(0.1f);
			if(m_bIsImgEnd){
				m_ImgFrame = 0;
				m_wstrStateKey = L"Idle";
			}
		} else{
			CGameObject::Animation();
			if(m_bIsWait){
				if(nullptr != m_pNextPath){
					//다음에 이동해야 하는 타일이 현재 이동 불가가 되었다면
					if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
						//찾은 경로 모두 삭제(다음 Update()에서 다시 찾을 수 있도록)
						SafeDelete(m_pNextPath);
						if(!m_PathList.empty()){
							for(auto& elem : m_PathList)
								SafeDelete(elem);
							m_PathList.clear();
						}
					} else{
						if(nullptr != m_pNextPath){
							//이동이 가능하다면 현재 다른 객체가 선점하고 있는지 체크
							m_pColTile->SetPreemTile(m_tInfo.vPos, this);
							const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
							if(nullptr == pObj){
								m_pColTile->SetPreemTile(*m_pNextPath, this);
								m_bIsWait = false;
								m_ImgFrame = 0;
								m_wstrStateKey = L"Walk";
								//대기상태가 너무 길어진다면 경로 삭제뒤, 정지상태로 전환(다른 객체들과 정면으로 충돌시 빠져나오게 함)
							} else if(m_fWaitTime > 0.5f){
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
	}
	CGameObject::UpdateAngle();
	CGameObject::UpdateRect();

	return S_OK;
}

HRESULT CPlayer::LateUpdate(){
	/////////////////////////////////////////////////////////////////////////////
	if(m_wstrStateKey != L"Die"){
		int iDamage = 0;
		if(iDamage = m_pColTile->GetDamage(m_tInfo.vPos)){
			if(rand() & 1){
				if(m_wstrStateKey == L"Idle"){
					m_ImgFrame = 0;
					m_wstrStateKey = L"Block";
				}
			} else
				if(m_wstrStateKey != L"Block"){
					if(m_wstrStateKey != L"Hit")
						m_pSoundMgr->PlaySound(L"Hit.wav", CSoundManager::PLAYER);
					SetDamage(iDamage);
				}
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	//목표 추적중 목표 위치 변경시
	if(nullptr != m_pTarget){
		D3DXVECTOR3 vTargetTilePos = m_pColTile->GetTile(m_pTarget->GetInfo().vPos)->vPos;
		if(!m_PathList.empty() && *m_PathList.back() != vTargetTilePos){
			m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
			if(!m_PathList.empty()){
				SafeDelete(m_PathList.back());
				m_PathList.pop_back();
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////
	//이동중 타일 선점 실패 대기시
	if(m_bIsWait){
		if(nullptr != m_pNextPath){
			//다음에 이동해야 하는 타일이 현재 이동 불가가 되었다면
			if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
				//찾은 경로 모두 삭제(다음 Update()에서 다시 찾을 수 있도록)
				SafeDelete(m_pNextPath);
				if(!m_PathList.empty()){
					for(auto& elem : m_PathList)
						SafeDelete(elem);
					m_PathList.clear();
				}
			} else{
				//이동이 가능하다면 현재 다른 객체가 선점하고 있는지 체크
				m_fWaitTime += m_pTimeMgr->GetDeltaTime();		//대기 시간 누적
				m_pColTile->SetPreemTile(m_tInfo.vPos, this);
				const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
				if(nullptr == pObj){
					m_pColTile->SetPreemTile(*m_pNextPath, this);
					m_bIsWait = false;
					m_ImgFrame = 0;
					m_wstrStateKey = L"Walk";
					//대기상태가 너무 길어진다면 경로 삭제뒤, 정지상태로 전환(다른 객체들과 정면으로 충돌시 빠져나오게 함)
				} else if(m_fWaitTime > 0.5f){
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
	/////////////////////////////////////////////////////////////////////////////

	m_pColTile->SetShadow(m_tInfo.vPos, 4);

	return S_OK;
}

HRESULT CPlayer::Render(){
	const TEXTURE_INFO* pTextureInfo = nullptr;

	D3DXMATRIX matScale, matTrans, matWorld;

	const D3DXVECTOR3 vScroll = m_pScrollMgr->GetScroll();

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
	matWorld = matScale * matTrans;

	pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);

	if(nullptr != pTextureInfo){
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		m_pDeviceMgr->GetSprite()->End();
	}

	if(m_pKeyMgr->IsKeyPressing(KEY_D)){
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

	return S_OK;
}

CPlayer * CPlayer::Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile, CTerrain* _pTerrain){
	CPlayer* pInstance = new CPlayer;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_pColTile = _pColTile;
	pInstance->m_pTerrain = _pTerrain;

	return pInstance;
}
