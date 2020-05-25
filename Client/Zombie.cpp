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
//�̵�
void CZombie::Move(){
	//���� �̵� ��ǥ�� ������
	if(nullptr != m_pNextPath){
		//�������� ���� ��ü�� �Ÿ� ���
		m_tInfo.vDir = *m_pNextPath - m_tInfo.vPos;
		float fLength = D3DXVec3Length(&m_tInfo.vDir);

		//���� ����� ���� ����ȭ
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		//�̵��Ÿ� ���
		D3DXVECTOR3 vSpeed = m_tInfo.vDir * 70.f * m_pTimeMgr->GetDeltaTime();

		//������������ �Ÿ��� ������� ����� ��
		if(fLength <= D3DXVec3Length(&vSpeed)){
			//���� ��ġ�� �������� �ٲ�
			m_tInfo.vPos = *m_pNextPath;
			SafeDelete(m_pNextPath);
			m_pColTile->SetPreemTile(m_tInfo.vPos, this);

			//��� ����Ʈ�� ������� �ʴٸ�, �� ������ �о��.
			if(!m_PathList.empty()){
				m_pNextPath = m_PathList.front();
				m_PathList.pop_front();
				//���� �̵� Ÿ�� �˻�
				if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
					//����Ÿ���� �̵� �Ұ��ϰ�, ��� ����Ʈ�� ���������
					if(m_PathList.empty()){
						SafeDelete(m_pNextPath);
						m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
					} else{		//���� ���� Ÿ���� �÷��̾��� ���
						if(m_pPlayer->GetInfo().vPos == *m_pNextPath){
							//���� Ÿ�ϰ� ��� ����Ʈ ����
							SafeDelete(m_pNextPath);
							for(auto& elem : m_PathList)
								SafeDelete(elem);
							m_PathList.clear();
							//���� ��ġ�� Ÿ�Ͽ� ���� �Ұ� ����
							m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
							//�÷��̾� �������� ���� ��ȯ
							D3DXVec3Normalize(&m_tInfo.vDir, &(m_pPlayer->GetInfo().vPos - m_tInfo.vPos));
							//������ ���°� �ƴҶ� ���� ���, ������ �����϶��� ��� ���
							if(!m_bIsDelayed){
								if(m_wstrStateKey != L"Attack")
									m_ImgFrame = 0;		//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
								m_wstrStateKey = L"Attack";
							} else{
								m_wstrStateKey = L"Idle";
							}
						} else{
							//���� Ÿ���� �̵��� �Ұ��� ��� ��� ��Ž��
							D3DXVECTOR3 vDest(*m_PathList.back());
							if(nullptr != m_pNextPath)
								m_pAstar->PathFinding(m_PathList, *m_pNextPath, vDest, 40);
							else
								m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, vDest, 40);

							if(!m_PathList.empty()){
								SafeDelete(m_pNextPath);
								m_pNextPath = m_PathList.front();
								m_PathList.pop_front();

								//Ÿ�� ���� �˻�
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
							} else{
								m_wstrStateKey = L"Idle";
								m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
							}
						}
					}
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
				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
			}
		} else{
			//������ �ʴٸ� ���������� �̵��ϰ� ��
			m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);
			m_tInfo.vPos += vSpeed;
			m_pColTile->SetPreemTile(m_tInfo.vPos, this);
		}
	} else{
		//�̵� ��ǥ�� �����Ƿ� ��� ���·� ��ȯ
		m_wstrStateKey = L"Idle";
		m_pColTile->SetPreemTile(m_tInfo.vPos, this);
		m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
	}

	CGameObject::Animation();
}
//����
void CZombie::Attack(){
	//���� ���� ��� ����� ������ ����
	if(m_bIsImgEnd){
		m_bIsDelayed = true;
		m_bIsAttack = false;

	} else if(m_ImgCount - 3 == CGameObject::Animation() && !m_bIsAttack){
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
		//��������Ʈ ó�� ���� �ؽ��� ���� ���� ����
		const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
		m_ImgCount = pTextureInfo->iImageCount;
		//���� ���°� �ƴҶ�
		if(m_wstrStateKey != L"Die"){
			//�÷��̾���� �Ÿ����
			float fDistance = D3DXVec3Length(&(m_tInfo.vPos - m_pPlayer->GetInfo().vPos));
			if(400.f > fDistance && (m_PathList.empty() || *m_PathList.back() != m_pColTile->GetTile(m_pPlayer->GetInfo().vPos)->vPos)){
				if(nullptr == m_pNextPath)
					m_pAstar->PathFinding(m_PathList, m_tInfo.vPos, m_pPlayer->GetInfo().vPos, 40);
				else
					m_pAstar->PathFinding(m_PathList, *m_pNextPath, m_pPlayer->GetInfo().vPos, 40);

				//��ã�� ��ΰ� ������ �ȱ� ������� ��ȯ
				if(!m_PathList.empty()){
					m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);
					m_pColTile->SetPreemTile(m_tInfo.vPos, this);
					//���� �̵����� �������� ��������� ���� ������ ����
					if(nullptr == m_pNextPath){
						m_pNextPath = m_PathList.front();
						m_PathList.pop_front();
					}

					//���� �̵� Ÿ�� �˻�
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

						if(m_wstrStateKey != L"Walk")
							m_ImgFrame = 0;		//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
						m_wstrStateKey = L"Walk";
					}
				} else{		//��ã�� ��ΰ� ������
					m_pColTile->SetPreemTile(m_tInfo.vPos, this);
					if(nullptr != m_pNextPath){		//�̵��ؾ��ϴ� ��ΰ� ������
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
					} else{
						m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);

						//������ �÷��̾� �������� ��ȯ
						D3DXVec3Normalize(&m_tInfo.vDir, &(m_pPlayer->GetInfo().vPos - m_tInfo.vPos));
						//������ ���°� �ƴҶ� ���� ���, ������ �����϶��� ��� ���
						if(!m_bIsDelayed){
							if(m_wstrStateKey != L"Attack")
								m_ImgFrame = 0;		//���� ���¿� ���� ���°� �ٸ� ��� ��������Ʈ ������ 0 �ʱ�ȭ
							m_wstrStateKey = L"Attack";
						} else{
							m_wstrStateKey = L"Idle";
						}
					}
				}
			}
		}

		//���� ��ǿ� ���� �ൿ ��ȯ
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

				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);		//�̵� �Ұ� ����
				m_pColTile->SetPreemTile(m_tInfo.vPos, nullptr);					//Ÿ�� ���� ����
				if(nullptr != m_pNextPath)
					m_pColTile->SetPreemTile(*m_pNextPath, nullptr);				//���� Ÿ�� ��������

				SafeDelete(m_pNextPath);											//�̵� Ÿ�� �Ҵ� ����
				for(auto& elem : m_PathList)										//��θ���Ʈ �Ҵ� ����
					SafeDelete(elem);
				m_PathList.clear();
			}
		} else{
			CGameObject::Animation();
			if(m_bIsDelayed){		//������ ������ ���
									//������ �ð� ���
				m_fCheckDelay += m_pTimeMgr->GetDeltaTime();
				//������ �ð��� �Ѿ�� ������ ���� ����
				if(m_fCheckDelay > 1.f){
					m_bIsDelayed = false;
					m_fCheckDelay = 0.f;
					m_ImgFrame = 0;			//������ ���� ������ ��������Ʈ ������ 0 �ʱ�ȭ
				}
			} else if(m_bIsWait){
				//�̵��� Ÿ�� ���� �����Ͽ� ����� �Ͻ�
				if(nullptr != m_pNextPath){
					//ã�� ��� ��� ����(���� Update()���� ��θ� �ٽ� ã�� �� �ֵ���)
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
						//�̵��� �����ϴٸ� ���� �ٸ� ��ü�� �����ϰ� �ִ��� üũ
						m_pColTile->SetPreemTile(m_tInfo.vPos, this);
						const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
						if(nullptr == pObj){
							m_pColTile->SetPreemTile(*m_pNextPath, this);
							m_bIsWait = false;
							m_ImgFrame = 0;
							m_wstrStateKey = L"Walk";
							//�����°� �ʹ� ������ٸ� ��� ������, �������·� ��ȯ(�ٸ� ��ü��� �������� �浹�� ���������� ��)
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

	//�̵��� Ÿ�� ���� �����Ͽ� ����� �Ͻ�
	if(m_bIsWait){
		if(nullptr != m_pNextPath){
			//������ �̵��ؾ� �ϴ� Ÿ���� ���� �̵� �Ұ��� �Ǿ��ٸ�
			if(!m_pColTile->GetTile(*m_pNextPath)->bIsColi){
				//ã�� ��� ��� ����(���� Update()���� ��θ� �ٽ� ã�� �� �ֵ���)
				SafeDelete(m_pNextPath);
				if(!m_PathList.empty()){
					for(auto& elem : m_PathList)
						SafeDelete(elem);
					m_PathList.clear();
				}
				m_pColTile->SetPreemTile(m_tInfo.vPos, this);
				m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
			} else{
				//�̵��� �����ϴٸ� ���� �ٸ� ��ü�� �����ϰ� �ִ��� üũ
				m_fWaitTime += m_pTimeMgr->GetDeltaTime();		//��� �ð� ����(LateUpdate()���� �ѹ��� ������(Update()������ ������ �������� ������)
				const CGameObject* const pObj = m_pColTile->GetPreemTile(*m_pNextPath);
				if(nullptr == pObj){
					m_pColTile->SetPreemTile(*m_pNextPath, this);
					m_bIsWait = false;
					m_ImgFrame = 0;
					m_wstrStateKey = L"Walk";
					//�����°� �ʹ� ������ٸ� ��� ������, �������·� ��ȯ(�ٸ� ��ü��� �������� �浹�� ���������� ��)
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
