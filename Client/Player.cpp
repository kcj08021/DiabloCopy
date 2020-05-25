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
//�̵�
void CPlayer::Move(){
	//���� �̵� ��ǥ�� ������
	if(nullptr != m_pNextPath){
		//�������� ���� ��ü�� �Ÿ� ���
		m_tInfo.vDir = *m_pNextPath - m_tInfo.vPos;
		float fLength = D3DXVec3Length(&m_tInfo.vDir);

		//���� ����� ���� ����ȭ
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		//�̵��Ÿ� ���
		D3DXVECTOR3 vSpeed = m_tInfo.vDir * 200.f * m_pTimeMgr->GetDeltaTime();

		//������������ �Ÿ��� �̵� �Ÿ����� ª�� ��
		if(fLength <= D3DXVec3Length(&vSpeed)){
			//���� ��ġ�� �������� �ٲ�
			m_tInfo.vPos = *m_pNextPath;
			m_pScrollMgr->SetScroll(m_pNextPath->x - (WINCX >> 1) + m_fScrollXDelta, m_pNextPath->y - (WINCY - 256)*0.5f);
			SafeDelete(m_pNextPath);

			if(!m_PathList.empty()){
				/*���⿡�� ���� Ÿ�� �˻� �ؾ��� �ƴϸ� LateUpdate()��*/
				m_pNextPath = m_PathList.front();
				m_PathList.pop_front();
				//�̵��ؾ��ϴ� ����Ÿ�� �˻�
				if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
					//���� Ÿ���� �̵��� �Ұ��� ��� ��� ��Ž��
					D3DXVECTOR3 vDest(*m_PathList.back());
					m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, vDest);
				} else{
					const CGameObject* const pPreemObj = m_pColTile->GetPreemTile(*m_pNextPath);
					if(this != pPreemObj && nullptr != pPreemObj){
						//���� Ÿ���� �ٸ� ��ü�� ���� �ϰ� �ִٸ� Wait�� true��
						m_bIsWait = true;
						m_wstrStateKey = L"Idle";
						m_ImgFrame = 0;
					} else{
						//���� Ÿ�� ����
						m_pColTile->SetPreemTile(*m_pNextPath, this);
					}
				}
			} else{
				//��������� ���� ��ġ �̵��Ұ� ���·� ��ȯ
				if(m_pNextPath == nullptr)
					m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
			}
		} else{
			//������ �ʴٸ� ���������� �̵��ϰ� ��
			m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);
			m_tInfo.vPos += vSpeed;
			m_pColTile->SetPreemTile(m_tInfo.vPos, this);

			//�÷��̾� ĳ���� �̵��� ��ũ�ѿ��� �̵��� �־���
			m_pScrollMgr->MoveScroll(vSpeed.x, vSpeed.y);
		}
	} else{
		if(nullptr != m_pTarget){
			//���� Ÿ�ٰ��� ���� ���
			if(m_pTarget->GetTypeKey() == L"Monster" || m_pTarget->GetTypeKey() == L"Boss"){
				D3DXVec3Normalize(&m_tInfo.vDir, &(m_pTarget->GetInfo().vPos - m_tInfo.vPos));
				if(m_wstrStateKey != L"Attack"){
					m_pSoundMgr->PlaySound(L"Attack.wav", CSoundManager::PLAYER);
					m_ImgFrame = 0;			//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
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
					m_ImgFrame = 0;			//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
				m_wstrStateKey = L"Idle";
			} else if(m_pTarget->GetTypeKey() == L"Object"){
				for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_MAPOBJ)){
					if(elem == m_pTarget){
						dynamic_cast<CMapObject*>(elem)->Activate();
						break;
					}
				}

				if(m_wstrStateKey != L"Idle")
					m_ImgFrame = 0;			//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
				m_wstrStateKey = L"Idle";
			}
		} else
			//�̵� ��ǥ�� �����Ƿ� ��� ���·� ��ȯ
			m_wstrStateKey = L"Idle";
	}

	CGameObject::Animation();
}
//����
void CPlayer::Attack(){
	int iFrame = CGameObject::Animation(0.033f);
	STAT_INFO tEquip;
	m_pHUD->GetEquipStat(&tEquip);
	if(m_ImgCount - 7 == iFrame && !m_bIsAttack){
		size_t index = m_pColTile->GetIndex(m_tInfo.vPos);
		//������ ���� �ε��� ���
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
		//Ÿ�Ͽ� ������ ����
		m_pColTile->SetDamageTile(index, m_tStat.iDamage + tEquip.iDamage);
		m_bIsAttack = true;
	} else if(m_bIsImgEnd){
		m_bIsAttack = false;
		//��Ŭ���� �����ϰ� �ִٸ� ��� ���� ���� ����, �ƴϸ� ��� ���·� ��ȯ
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

	//�÷��̾� ���� ����ü
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
		//��������Ʈ ó�� ���� �ؽ��� ���� ���� ����
		const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
		m_ImgCount = pTextureInfo->iImageCount;

		//ĳ���� ���� �κ�
		/////////////////////////////////////////////////////////////////////////////
		D3DXVECTOR3 vMousePos = m_pKeyMgr->GetMousePos();		//���콺 ��ǥ

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
					//��Ŭ���� ���� ��ǥ�� ������ ������Ʈ �����͸� ���
					m_pTarget = nullptr;

					//����Ʈ ���� ���¿��� ��Ŭ����
					if(m_pKeyMgr->IsKeyPressing(KEY_SHIFT)){
						//��� ����Ʈ�� ��� ���� �� �ʱ�ȭ
						for(auto& elem : m_PathList)
							SafeDelete(elem);
						m_PathList.clear();

						//���콺 Ŭ�� ������ ���� ��ü ��ġ�� ���� ���
						D3DXVec3Normalize(&m_tInfo.vDir, &(m_pKeyMgr->GetMousePos() - (m_tInfo.vPos - m_pScrollMgr->GetScroll())));

						//���� �̵����� �������� ���ٸ� ���� ���·� ��ȯ
						if(nullptr == m_pNextPath){
							if(m_wstrStateKey != L"Attack"){
								m_ImgFrame = 0;			//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
								m_pSoundMgr->PlaySound(L"Attack.wav", CSoundManager::PLAYER);
							}
							m_wstrStateKey = L"Attack";
						}
						/////////////////////////////////////////////////////////////////////////////
					} else{
						//�׳� ��Ŭ���� Ŭ��
						if(!bIsLevelUpClicked){
							//Ŭ���� ������ ������Ʈ�� �ִ��� �Ǻ�, �ִٸ� ��ǥ�� ����
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


							//�ش� �������� ��ã��
							//���� ��ġ �̵��Ұ� ����
							m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);
							if(nullptr == m_pTarget){
								if(nullptr != m_pNextPath)
									m_pAstar->PathFinding(m_PathList, *m_pNextPath, vMousePos + m_pScrollMgr->GetScroll());
								else
									m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, vMousePos + m_pScrollMgr->GetScroll());
							} else{
								//��ǥ�� ������ ��ǥ �ٷ� �� �������� ��η� ����
								if(nullptr != m_pNextPath)
									m_pAstar->PathFinding(m_PathList, *m_pNextPath, m_pTarget->GetInfo().vPos);
								else
									m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, m_pTarget->GetInfo().vPos);
							}
							//��� ����Ʈ�� ������� ������
							if(!m_PathList.empty()){
								//���� �̵����� �������� ��������� ���� ������ ����

								if(nullptr == m_pNextPath){
									m_pNextPath = m_PathList.front();
									m_PathList.pop_front();

									const CGameObject* const pPreemObj = m_pColTile->GetPreemTile(*m_pNextPath);
									if(this != pPreemObj && nullptr != pPreemObj){
										//���� Ÿ���� �ٸ� ��ü�� ���� �ϰ� �ִٸ� Wait�� true��
										m_bIsWait = true;
										m_fWaitTime = 0.f;
										m_wstrStateKey = L"Idle";
										m_ImgFrame = 0;
									} else{
										//���� Ÿ�� ����
										m_pColTile->SetPreemTile(*m_pNextPath, this);
									}
								}
								if(!m_bIsWait){
									if(m_wstrStateKey != L"Walk")
										m_ImgFrame = 0;			//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
									m_wstrStateKey = L"Walk";
								}
							} else{
								if(m_pNextPath == nullptr)
									m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
								if(nullptr != m_pTarget){
									if(m_pTarget->GetTypeKey() == L"Monster" || m_pTarget->GetTypeKey() == L"Boss"){ 	//�ش� Ÿ���� ������ ���
										D3DXVec3Normalize(&m_tInfo.vDir, &(m_pTarget->GetInfo().vPos - m_tInfo.vPos));
										if(m_wstrStateKey != L"Attack"){
											m_ImgFrame = 0;			//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
											m_pSoundMgr->PlaySound(L"Attack.wav", CSoundManager::PLAYER);
										}
										m_wstrStateKey = L"Attack";
									} else if(m_pTarget->GetTypeKey() == L"Item"){ //�ش� Ÿ���� �������� ���
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
											m_ImgFrame = 0;			//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
										m_wstrStateKey = L"Idle";
									}
								}
							}
						}
					}
				}
				/////////////////////////////////////////////////////////////////////////////
				//��Ŭ���� ���� ���
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


		//���� ��ǿ� ���� �ൿ �Լ� �۵�
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

				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);		//�̵� �Ұ� ����
				m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);					//Ÿ�� ���� ����
				if(nullptr != m_pNextPath)
					m_pColTile->SetPreemTile(*m_pNextPath, nullptr);				//���� Ÿ�� ��������

				SafeDelete(m_pNextPath);											//�̵� Ÿ�� �Ҵ� ����
				for(auto& elem : m_PathList)										//��θ���Ʈ �Ҵ� ����
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
					//������ �̵��ؾ� �ϴ� Ÿ���� ���� �̵� �Ұ��� �Ǿ��ٸ�
					if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
						//ã�� ��� ��� ����(���� Update()���� �ٽ� ã�� �� �ֵ���)
						SafeDelete(m_pNextPath);
						if(!m_PathList.empty()){
							for(auto& elem : m_PathList)
								SafeDelete(elem);
							m_PathList.clear();
						}
					} else{
						if(nullptr != m_pNextPath){
							//�̵��� �����ϴٸ� ���� �ٸ� ��ü�� �����ϰ� �ִ��� üũ
							m_pColTile->SetPreemTile(m_tInfo.vPos, this);
							const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
							if(nullptr == pObj){
								m_pColTile->SetPreemTile(*m_pNextPath, this);
								m_bIsWait = false;
								m_ImgFrame = 0;
								m_wstrStateKey = L"Walk";
								//�����°� �ʹ� ������ٸ� ��� ������, �������·� ��ȯ(�ٸ� ��ü��� �������� �浹�� ���������� ��)
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
	//��ǥ ������ ��ǥ ��ġ �����
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
	//�̵��� Ÿ�� ���� ���� ����
	if(m_bIsWait){
		if(nullptr != m_pNextPath){
			//������ �̵��ؾ� �ϴ� Ÿ���� ���� �̵� �Ұ��� �Ǿ��ٸ�
			if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
				//ã�� ��� ��� ����(���� Update()���� �ٽ� ã�� �� �ֵ���)
				SafeDelete(m_pNextPath);
				if(!m_PathList.empty()){
					for(auto& elem : m_PathList)
						SafeDelete(elem);
					m_PathList.clear();
				}
			} else{
				//�̵��� �����ϴٸ� ���� �ٸ� ��ü�� �����ϰ� �ִ��� üũ
				m_fWaitTime += m_pTimeMgr->GetDeltaTime();		//��� �ð� ����
				m_pColTile->SetPreemTile(m_tInfo.vPos, this);
				const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
				if(nullptr == pObj){
					m_pColTile->SetPreemTile(*m_pNextPath, this);
					m_bIsWait = false;
					m_ImgFrame = 0;
					m_wstrStateKey = L"Walk";
					//�����°� �ʹ� ������ٸ� ��� ������, �������·� ��ȯ(�ٸ� ��ü��� �������� �浹�� ���������� ��)
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
