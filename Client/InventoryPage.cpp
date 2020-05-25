#include "stdafx.h"
#include "InventoryPage.h"
#include "Player.h"
#include "HUD.h"
#include "Item.h"
#include "Gold.h"
#include "HeavyArmor.h"
#include "Sword.h"
#include "Helmet.h"
#include "Shield.h"
#include "Amulet.h"
#include "Ring.h"
#include "Potion.h"

CInventoryPage::CInventoryPage(): m_pKeyMgr(KEYMGR), m_pSelectedItem(nullptr), m_bisItemSelected(false){
	INIT_ARRAY(m_tEquip);
	INIT_ARRAY(m_tInven);
	INIT_ARRAY(m_InvenItem);
	INIT_ARRAY(m_bIsRealItem);
}


CInventoryPage::~CInventoryPage(){
	Release();
}

void CInventoryPage::CreateEquipRect(){
	m_tEquip[HEAD] = { static_cast<LONG>(m_tInfo.vPos.x) + 266 , 6, static_cast<LONG>(m_tInfo.vPos.x) + 378, 118 };
	m_tEquip[BODY] = { static_cast<LONG>(m_tInfo.vPos.x) + 266 , 154 ,static_cast<LONG>(m_tInfo.vPos.x) + 378, 322 };
	m_tEquip[WEAPON] = { static_cast<LONG>(m_tInfo.vPos.x) + 36, 154, static_cast<LONG>(m_tInfo.vPos.x) + 148, 322 };
	m_tEquip[SHIELD] = { static_cast<LONG>(m_tInfo.vPos.x) + 498, 154, static_cast<LONG>(m_tInfo.vPos.x) + 610, 322 };
	m_tEquip[NECK] = { static_cast<LONG>(m_tInfo.vPos.x) + 410, 66, static_cast<LONG>(m_tInfo.vPos.x) + 466, 122 };
	m_tEquip[RING1] = { static_cast<LONG>(m_tInfo.vPos.x) + 94, 356, static_cast<LONG>(m_tInfo.vPos.x) + 150, 412 };
	m_tEquip[RING2] = { static_cast<LONG>(m_tInfo.vPos.x) + 496, 356, static_cast<LONG>(m_tInfo.vPos.x) + 552, 412 };
}

void CInventoryPage::CreateInvenRect(){
	m_tInven[0][0] = { static_cast<LONG>(m_tInfo.vPos.x) + 34, 444, static_cast<LONG>(m_tInfo.vPos.x) + 90, 500 };

	for(int i = 0; i < INVEN_HIEGTH; ++i){
		for(int j = 0; j < INVEN_WIDTH; ++j){
			m_tInven[i][j] = { static_cast<LONG>(m_tInfo.vPos.x) + 34 + ((2 + 56)*j), 444 + ((2 + 56)*i), static_cast<LONG>(m_tInfo.vPos.x) + 90 + ((2 + 56)*j), 500 + ((2 + 56)*i) };
		}
	}
}

void CInventoryPage::CreateDefualtEquip(){
	m_pEquipItem[HEAD] = CHelmet::Create();
	m_pEquipItem[BODY] = CHeavyArmor::Create();
	m_pEquipItem[WEAPON] = CSword::Create();
	m_pEquipItem[SHIELD] = CShield::Create();
	m_pEquipItem[NECK] = CAmulet::Create();
	m_pEquipItem[RING1] = CRing::Create();
	m_pEquipItem[RING2] = CRing::Create();

	m_pEquipItem[HEAD]->SetIndex(0);
	m_pEquipItem[BODY]->SetIndex(0);
	m_pEquipItem[WEAPON]->SetIndex(0);
	m_pEquipItem[SHIELD]->SetIndex(0);
	m_pEquipItem[NECK]->SetIndex(0);
	m_pEquipItem[RING1]->SetIndex(0);
	m_pEquipItem[RING2]->SetIndex(0);
}

int CInventoryPage::GetGold(){
	for(int i = 0; i < INVEN_HIEGTH; ++i)
		for(int j = 0; j < INVEN_WIDTH; ++j)
			if(m_InvenItem[i][j] != nullptr &&m_InvenItem[i][j]->GetObjectKey() == L"Gold")
				return dynamic_cast<CGold*>(m_InvenItem[i][j])->GetGold();

	return 0;
}

void CInventoryPage::GetEquipStat(STAT_INFO* _info){
	for(auto& elem : m_pEquipItem){
		_info->iArmor += elem->GetStat().iArmor;
		_info->iDamage += elem->GetStat().iDamage;
	}
}

HRESULT CInventoryPage::AddItem(CItem* _pItem){
	if(_pItem->GetObjectKey() == L"Gold")
		for(int i = 0; i < INVEN_HIEGTH; ++i)
			for(int j = 0; j < INVEN_WIDTH; ++j)
				if(m_InvenItem[i][j] != nullptr && m_InvenItem[i][j]->GetObjectKey() == L"Gold"){
					dynamic_cast<CGold*>(m_InvenItem[i][j])->AddGold(dynamic_cast<CGold*>(_pItem)->GetGold());
					SafeDelete(_pItem);

					return S_OK;
				}
	
	POINT ptItemSize = {};
	_pItem->GetItemSize(ptItemSize);

	for(int i = 0; i < INVEN_HIEGTH; ++i){
		if(i + ptItemSize.y > INVEN_HIEGTH)
			break;
		for(int j = 0; j < INVEN_WIDTH; ++j){
			if(j + ptItemSize.x > INVEN_WIDTH)
				break;

			bool bIsFail = false;
			if(nullptr == m_InvenItem[i][j]){
				for(int iSizeY = 0; iSizeY < ptItemSize.y; ++iSizeY)
					for(int iSizeX = 0; iSizeX < ptItemSize.x; ++iSizeX)
						if(nullptr != m_InvenItem[i + iSizeY][j + iSizeX]){
							bIsFail = true;
						}

				if(!bIsFail){
					m_bIsRealItem[i][j] = true;
					for(int iSizeY = 0; iSizeY < ptItemSize.y; ++iSizeY)
						for(int iSizeX = 0; iSizeX < ptItemSize.x; ++iSizeX)
							m_InvenItem[i + iSizeY][j + iSizeX] = _pItem;

					return S_OK;
				} else{
					continue;
				}
			}
		}
	}

	return E_FAIL;
}

HRESULT CInventoryPage::Release(){
	for(int i = 0; i < INVEN_HIEGTH; ++i)
		for(int j = 0; j < INVEN_WIDTH; ++j){
			if(m_bIsRealItem[i][j])
				SafeDelete(m_InvenItem[i][j]);
			else
				m_InvenItem[i][j] = nullptr;
		}

	for(CItem*& elem : m_pEquipItem)
		SafeDelete(elem);

	SafeDelete(m_pSelectedItem);

	return S_OK;
}

HRESULT CInventoryPage::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { static_cast<float>(WINCX >> 1), 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"UI";
	m_wstrObjectKey = L"InventoryPage";

	m_bIsVisible = false;

	CreateEquipRect();
	CreateInvenRect();
	CreateDefualtEquip();

	return S_OK;
}

HRESULT CInventoryPage::LateInit(){
	return S_OK;
}

HRESULT CInventoryPage::Update(){
	D3DXVECTOR3 vMousePos = m_pKeyMgr->GetMousePos();

	if(m_bIsVisible){
		if(!m_bisItemSelected){
			if(vMousePos.x > static_cast<float>(WINCX - (WINCX >> 1)) && vMousePos.x < static_cast<float>(WINCX) &&
				vMousePos.y > 0.f && vMousePos.y < WINCY - 282){
				wstring wstrInfo = L"";
				if(wstrInfo == L"")
					for(int i = 0; i < EQUIP_END; ++i){
						if(PtInRect(&m_tEquip[i], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
							if(nullptr != m_pEquipItem[i]){
								wstrInfo = m_pEquipItem[i]->GetStat().szName;
								if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
									m_pSelectedItem = m_pEquipItem[i];
									m_bisItemSelected = true;
									m_pEquipItem[i] = nullptr;
								}
							}
							break;
						}
					}

				if(wstrInfo == L"")
					for(int i = 0; i < INVEN_HIEGTH; ++i){
						for(int j = 0; j < INVEN_WIDTH; ++j)
							if(PtInRect(&m_tInven[i][j], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
								if(nullptr != m_InvenItem[i][j]){
									if(m_InvenItem[i][j]->GetObjectKey() == L"Gold")
										m_InvenItem[i][j]->LateUpdate();
									wstrInfo = m_InvenItem[i][j]->GetStat().szName;
									if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
										m_pSelectedItem = m_InvenItem[i][j];
										m_bisItemSelected = true;
										for(int k = 0; k < INVEN_HIEGTH; ++k)
											for(int l = 0; l < INVEN_WIDTH; ++l){
												if(m_InvenItem[k][l] == m_pSelectedItem){
													m_InvenItem[k][l] = nullptr;
													m_bIsRealItem[k][l] = false;
												}
											}
									}
								}
								break;
							}
					}

				m_pHUD->SetInfoText(wstrInfo);
			}
		} else{
			wstring wstrSelectedItem = m_pSelectedItem->GetObjectKey();
			if(wstrSelectedItem == L"Helmet"){
				if(PtInRect(&m_tEquip[HEAD], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						if(nullptr == m_pEquipItem[HEAD]){
							m_pEquipItem[HEAD] = m_pSelectedItem;
							m_pSelectedItem = nullptr;
							m_bisItemSelected = false;
						} else{
							CItem* temp = m_pEquipItem[HEAD];
							m_pEquipItem[HEAD] = m_pSelectedItem;
							m_pSelectedItem = temp;
						}
					}
				}
			} else if(wstrSelectedItem == L"HeavyArmor"){
				if(PtInRect(&m_tEquip[BODY], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						if(nullptr == m_pEquipItem[BODY]){
							m_pEquipItem[BODY] = m_pSelectedItem;
							m_pSelectedItem = nullptr;
							m_bisItemSelected = false;
						} else{
							CItem* temp = m_pEquipItem[BODY];
							m_pEquipItem[BODY] = m_pSelectedItem;
							m_pSelectedItem = temp;
						}
					}
				}
			} else if(wstrSelectedItem == L"Sword"){
				if(PtInRect(&m_tEquip[WEAPON], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						if(nullptr == m_pEquipItem[WEAPON]){
							m_pEquipItem[WEAPON] = m_pSelectedItem;
							m_pSelectedItem = nullptr;
							m_bisItemSelected = false;
						} else{
							CItem* temp = m_pEquipItem[WEAPON];
							m_pEquipItem[WEAPON] = m_pSelectedItem;
							m_pSelectedItem = temp;
						}
					}
				}
			} else if(wstrSelectedItem == L"Shield"){
				if(PtInRect(&m_tEquip[SHIELD], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						if(nullptr == m_pEquipItem[SHIELD]){
							m_pEquipItem[SHIELD] = m_pSelectedItem;
							m_pSelectedItem = nullptr;
							m_bisItemSelected = false;
						} else{
							CItem* temp = m_pEquipItem[SHIELD];
							m_pEquipItem[SHIELD] = m_pSelectedItem;
							m_pSelectedItem = temp;
						}
					}
				}
			} else if(wstrSelectedItem == L"Amulet"){
				if(PtInRect(&m_tEquip[NECK], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						if(nullptr == m_pEquipItem[NECK]){
							m_pEquipItem[NECK] = m_pSelectedItem;
							m_pSelectedItem = nullptr;
							m_bisItemSelected = false;
						} else{
							CItem* temp = m_pEquipItem[NECK];
							m_pEquipItem[NECK] = m_pSelectedItem;
							m_pSelectedItem = temp;
						}
					}
				}
			} else if(wstrSelectedItem == L"Ring"){
				if(PtInRect(&m_tEquip[RING1], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						if(nullptr == m_pEquipItem[RING1]){
							m_pEquipItem[RING1] = m_pSelectedItem;
							m_pSelectedItem = nullptr;
							m_bisItemSelected = false;
						} else{
							CItem* temp = m_pEquipItem[RING1];
							m_pEquipItem[RING1] = m_pSelectedItem;
							m_pSelectedItem = temp;
						}
					}
				} else if(PtInRect(&m_tEquip[RING2], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						if(nullptr == m_pEquipItem[RING2]){
							m_pEquipItem[RING2] = m_pSelectedItem;
							m_pSelectedItem = nullptr;
							m_bisItemSelected = false;
						} else{
							CItem* temp = m_pEquipItem[RING2];
							m_pEquipItem[RING2] = m_pSelectedItem;
							m_pSelectedItem = temp;
						}
					}
				}
			}

			if(m_bisItemSelected){
				POINT ptItemSize = {};
				m_pSelectedItem->GetItemSize(ptItemSize);
				for(int i = 0; i < INVEN_HIEGTH; ++i){
					if(i + ptItemSize.y > INVEN_HIEGTH)
						break;
					for(int j = 0; j < INVEN_WIDTH; ++j){
						if(j + ptItemSize.x > INVEN_WIDTH)
							break;

						if(PtInRect(&m_tInven[i][j], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) })){
							bool bIsFail = false;
							CItem* changeItem = nullptr;
							if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
								for(int iSizeY = 0; iSizeY < ptItemSize.y; ++iSizeY)
									for(int iSizeX = 0; iSizeX < ptItemSize.x; ++iSizeX)
										if(nullptr != m_InvenItem[i + iSizeY][j + iSizeX]){
											if(nullptr == changeItem)
												changeItem = m_InvenItem[i + iSizeY][j + iSizeX];
											else{
												if(changeItem != m_InvenItem[i + iSizeY][j + iSizeX])
													bIsFail = true;
											}
										}

								if(!bIsFail){
									CItem* tempItem = m_pSelectedItem;
									if(nullptr == changeItem){
										m_pSelectedItem = nullptr;
										m_bisItemSelected = false;
									} else{
										for(int i = 0; i < INVEN_HIEGTH; ++i)
											for(int j = 0; j < INVEN_WIDTH; ++j){
												if(changeItem == m_InvenItem[i][j]){
													m_InvenItem[i][j] = nullptr;
													m_bIsRealItem[i][j] = false;
												}
											}
										m_pSelectedItem = changeItem;
									}

									for(int iSizeY = 0; iSizeY < ptItemSize.y; ++iSizeY)
										for(int iSizeX = 0; iSizeX < ptItemSize.x; ++iSizeX){
											m_InvenItem[i + iSizeY][j + iSizeX] = tempItem;
											m_bIsRealItem[i + iSizeY][j + iSizeX] = false;
										}
									m_bIsRealItem[i][j] = true;
								}
							}
						}
					}
				}

				if(vMousePos.x > 0.f && vMousePos.x < static_cast<float>(WINCX >> 1) && vMousePos.y > 0.f && vMousePos.y < WINCY - 282){
					if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
						m_pSelectedItem->SetPosition(m_pPlayer->GetInfo().vPos);
						m_pSelectedItem->SetAinimationInit();
						m_pSelectedItem->SetSoundInit();
						OBJMGR->AddObject(CObjectManager::OBJ_ITEM, m_pSelectedItem);

						m_pSelectedItem = nullptr;
						m_bisItemSelected = false;
					}
				}

				CPotion* pPotion = dynamic_cast<CPotion*>(m_pSelectedItem);
				if(nullptr != pPotion){
					for(int i = 0; i < 8; ++i){
						if(PtInRect(&m_pHUD->GetQuickRect(i), POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) }))
							if(m_pKeyMgr->IsKeyUp(KEY_LBUTTON)){
								m_pSelectedItem = m_pHUD->SetQuickItem(i, pPotion);
								if(nullptr == m_pSelectedItem)
									m_bisItemSelected = false;
								break;
							}
					}
				}
			}
		}
	} else{
		if(m_bisItemSelected){
			if(vMousePos.y > 0.f && vMousePos.y < WINCY - 282){
				if(m_pKeyMgr->IsKeyUp(KEY_LBUTTON)){
					m_pSelectedItem->SetPosition(m_pPlayer->GetInfo().vPos);
					m_pSelectedItem->SetAinimationInit();
					OBJMGR->AddObject(CObjectManager::OBJ_ITEM, m_pSelectedItem);

					m_pSelectedItem = nullptr;
					m_bisItemSelected = false;
				}
			}

			CPotion* pPotion = dynamic_cast<CPotion*>(m_pSelectedItem);
			if(nullptr != pPotion){
				for(int i = 0; i < 8; ++i){
					if(PtInRect(&m_pHUD->GetQuickRect(i), POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) }))
						if(m_pKeyMgr->IsKeyUp(KEY_LBUTTON)){
							m_pSelectedItem = m_pHUD->SetQuickItem(i, pPotion);
							if(nullptr == m_pSelectedItem)
								m_bisItemSelected = false;
							break;
						}
				}
			}
		}
	}

	if(m_bisItemSelected){
		m_pHUD->SetInfoText(m_pSelectedItem->GetStat().szName);
		if(!m_bIsSoundOut){
			m_bIsSoundOut = true;
			if(m_pSelectedItem->GetObjectKey() == L"Gold")
				m_pSoundMgr->PlaySound(L"Gold.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"Helmet")
				m_pSoundMgr->PlaySound(L"InvenShield.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"HeavyArmor")
				m_pSoundMgr->PlaySound(L"InvenShield.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"Shield")
				m_pSoundMgr->PlaySound(L"InvenShield.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"Sword")
				m_pSoundMgr->PlaySound(L"InvenSword.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"Ring")
				m_pSoundMgr->PlaySound(L"InvenRing.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"Amulet")
				m_pSoundMgr->PlaySound(L"InvenRing.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"HPPotion")
				m_pSoundMgr->PlaySound(L"InvenPotion.wav", CSoundManager::EFFECT);
			else if(m_pSelectedItem->GetObjectKey() == L"MPPotion")
				m_pSoundMgr->PlaySound(L"InvenPotion.wav", CSoundManager::EFFECT);
		}
	} else
		m_bIsSoundOut = false;
	return S_OK;
}

HRESULT CInventoryPage::LateUpdate(){
	m_pPlayer->SetItemDrop(m_bisItemSelected);

	return S_OK;
}

HRESULT CInventoryPage::Render(){
	const TEXTURE_INFO* pTextureInfo = nullptr;

	D3DXMATRIX matScale, matTrans, matWorld;

	if(m_bIsVisible){
		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
		matWorld = matScale * matTrans;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);

		if(nullptr != pTextureInfo){
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pDeviceMgr->GetSprite()->End();
		}

		for(int i = 0; i < EQUIP_END; ++i){
			if(nullptr != m_pEquipItem[i]){
				pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Item", m_pEquipItem[i]->GetObjectKey(), m_pEquipItem[i]->GetIndex(), L"Inventory");

				float faddX = (m_tEquip[i].right - m_tEquip[i].left - (pTextureInfo->tImgInfo.Width*2.f))*0.5f;
				float faddY = (m_tEquip[i].bottom - m_tEquip[i].top - (pTextureInfo->tImgInfo.Height*2.f))*0.5f;


				if(nullptr != pTextureInfo){
					INIT_STRUCT(matScale);
					INIT_STRUCT(matTrans);

					D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
					D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tEquip[i].left) + faddX, static_cast<float>(m_tEquip[i].top) + faddY, 0.f);
					matWorld = matScale * matTrans;

					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
					m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

					m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

					m_pDeviceMgr->GetSprite()->End();
				}
			}
		}

		for(int i = 0; i < INVEN_HIEGTH; ++i)
			for(int j = 0; j < INVEN_WIDTH; ++j){
				if(nullptr != m_InvenItem[i][j]){
					pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Item", L"InventoryCell");

					if(nullptr != pTextureInfo){
						INIT_STRUCT(matScale);
						INIT_STRUCT(matTrans);

						D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
						D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tInven[i][j].left), static_cast<float>(m_tInven[i][j].top), 0.f);
						matWorld = matScale * matTrans;

						m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
						m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

						m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						m_pDeviceMgr->GetSprite()->End();
					}
				}
			}

		for(int i = 0; i < INVEN_HIEGTH; ++i)
			for(int j = 0; j < INVEN_WIDTH; ++j){
				if(m_bIsRealItem[i][j]){
					pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Item", m_InvenItem[i][j]->GetObjectKey(), m_InvenItem[i][j]->GetIndex(), L"Inventory");

					if(nullptr != pTextureInfo){
						INIT_STRUCT(matScale);
						INIT_STRUCT(matTrans);

						D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
						D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tInven[i][j].left), static_cast<float>(m_tInven[i][j].top), 0.f);
						matWorld = matScale * matTrans;

						m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
						m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

						m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

						m_pDeviceMgr->GetSprite()->End();
					}
				}
			}


		if(m_pKeyMgr->IsKeyPressing(KEY_D)){
			for(const RECT& rc : m_tEquip){
				D3DXVECTOR2 pt[5] = { { static_cast<float>(rc.left), static_cast<float>(rc.top) },
				{ static_cast<float>(rc.right), static_cast<float>(rc.top) },
				{ static_cast<float>(rc.right), static_cast<float>(rc.bottom) },
				{ static_cast<float>(rc.left), static_cast<float>(rc.bottom) },
				{ static_cast<float>(rc.left), static_cast<float>(rc.top) }
				};

				m_pDeviceMgr->GetLine()->Begin();
				m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_pDeviceMgr->GetLine()->End();
			}

			for(auto& rcArray : m_tInven)
				for(auto& rc : rcArray){
					D3DXVECTOR2 pt[5] = { { static_cast<float>(rc.left), static_cast<float>(rc.top) },
					{ static_cast<float>(rc.right), static_cast<float>(rc.top) },
					{ static_cast<float>(rc.right), static_cast<float>(rc.bottom) },
					{ static_cast<float>(rc.left), static_cast<float>(rc.bottom) },
					{ static_cast<float>(rc.left), static_cast<float>(rc.top) }
					};

					m_pDeviceMgr->GetLine()->Begin();
					m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
					m_pDeviceMgr->GetLine()->End();
				}
		}
	}

	if(m_bisItemSelected){
		if(nullptr != m_pSelectedItem){
			pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Item", m_pSelectedItem->GetObjectKey(), m_pSelectedItem->GetIndex(), L"Inventory");

			if(nullptr != pTextureInfo){
				INIT_STRUCT(matScale);
				INIT_STRUCT(matTrans);

				D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
				D3DXMatrixTranslation(&matTrans, m_pKeyMgr->GetMousePos().x, m_pKeyMgr->GetMousePos().y, 0.f);
				matWorld = matScale * matTrans;

				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

				m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

				m_pDeviceMgr->GetSprite()->End();
			}
		}
	}

	return S_OK;
}

CInventoryPage * CInventoryPage::Create(CPlayer * _pPlayer, CHUD* _pHUD){
	CInventoryPage* pInstance = new CInventoryPage;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pHUD = _pHUD;

	return pInstance;
}
