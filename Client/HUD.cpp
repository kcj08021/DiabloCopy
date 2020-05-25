#include "stdafx.h"
#include "HUD.h"
#include "Player.h"
#include "Terrain.h"
#include "StatusPage.h"
#include "InventoryPage.h"
#include "QuestPage.h"
#include "SpellPage.h"
#include "Spell.h"
#include "Item.h"
#include "Potion.h"

CHUD::CHUD(): m_pPlayer(nullptr), m_pKeyMgr(KEYMGR), m_bIsLevelUp(false), m_pDisplaySpell(nullptr){
	INIT_STRUCT(m_tLevelUpButton);
	INIT_STRUCT(m_tLevelUpText);

	INIT_STRUCT(m_tHpBar);
	INIT_STRUCT(m_tMpBar);
	INIT_STRUCT(m_tInfoRect);


	INIT_ARRAY(m_tButtonRect);
	INIT_ARRAY(m_pPage);
	INIT_ARRAY(m_tQuick);
	INIT_ARRAY(m_pQuick);
}


CHUD::~CHUD(){
	Release();
}

void CHUD::SpellRectCreate(){
	RECT* rc = nullptr;

	for(auto& elem : m_pSpellRect)
		SafeDelete(elem);
	m_pSpellRect.clear();
	m_pSpellRect.shrink_to_fit();

	for(size_t i = 0; i < m_SpellList.size(); ++i)
		if(m_pSpellRect.empty()){
			rc = new RECT;
			rc->right = WINCX - 40;
			rc->bottom = WINCY - 300;
			rc->left = rc->right - 112;
			rc->top = rc->bottom - 112;
			m_pSpellRect.push_back(rc);
		} else{
			rc = new RECT;
			rc->right = m_pSpellRect.back()->left - 10;
			rc->bottom = WINCY - 300;
			rc->left = rc->right - 112;
			rc->top = rc->bottom - 112;

			if(rc->left < 0){
				rc->right = WINCX - 40;
				rc->bottom = rc->top - 10;
				rc->left = rc->right - 112;
				rc->top = rc->bottom - 112;
			}

			m_pSpellRect.push_back(rc);
		}


}

void CHUD::LevelUp(){
	if(!m_pPage[STATUS]->IsVisible())
		m_bIsLevelUp = true;

	dynamic_cast<CStatusPage*>(m_pPage[STATUS])->SetStatPoint();
}

void CHUD::UseQuick(size_t _iIndex){
	if(m_pQuick[_iIndex] != nullptr){
		m_pQuick[_iIndex]->PotionUsed();
		SafeDelete(m_pQuick[_iIndex]);
	}
}

HRESULT CHUD::AddItem(CGameObject* _pItem){
	if(_pItem->GetObjectKey().substr(2) == L"Potion"){
		for(auto& elem : m_pQuick)
			if(nullptr == elem){
				elem = dynamic_cast<CPotion*>(_pItem);
				return S_OK;
			}
	}

	return dynamic_cast<CInventoryPage*>(m_pPage[INVENTORY])->AddItem(dynamic_cast<CItem*>(_pItem));
}

int CHUD::GetGold(){

	return dynamic_cast<CInventoryPage*>(m_pPage[INVENTORY])->GetGold();
}

void CHUD::GetEquipStat(STAT_INFO* _info){
	dynamic_cast<CInventoryPage*>(m_pPage[INVENTORY])->GetEquipStat(_info);
}

CPotion * CHUD::SetQuickItem(size_t _Index, CPotion * _pItem){
	CPotion* tempItem = nullptr;

	if(nullptr == m_pQuick[_Index])
		m_pQuick[_Index] = _pItem;
	else{
		tempItem = m_pQuick[_Index];
		m_pQuick[_Index] = _pItem;
	}

	return tempItem;
}

HRESULT CHUD::Release(){
	for(auto& elem : m_pQuick)
		SafeDelete(elem);

	for(auto& elem : m_pSpellRect)
		SafeDelete(elem);

	return S_OK;
}

HRESULT CHUD::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, WINCY - 282, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"UI";
	m_wstrObjectKey = L"HUD";

	m_tButtonRect[STATUS] = { 12, static_cast<LONG>(m_tInfo.vPos.y) + 38, 166, static_cast<LONG>(m_tInfo.vPos.y) + 88 };
	m_tButtonRect[QUEST] = { 12, static_cast<LONG>(m_tInfo.vPos.y) + 92, 166, static_cast<LONG>(m_tInfo.vPos.y) + 138 };
	m_tButtonRect[MAP] = { 12, WINCY - 112 , 166, WINCY - 66 };
	m_tButtonRect[MENU] = { 12, WINCY - 62, 166, WINCY - 12 };

	m_tButtonRect[INVENTORY] = { WINCX - 166, static_cast<LONG>(m_tInfo.vPos.y) + 38, WINCX - 12, static_cast<LONG>(m_tInfo.vPos.y) + 88 };
	m_tButtonRect[SPELL] = { WINCX - 166, static_cast<LONG>(m_tInfo.vPos.y) + 92, WINCX - 12, static_cast<LONG>(m_tInfo.vPos.y) + 138 };
	m_tButtonRect[QUICK] = { WINCX - 148, WINCY - 126 , WINCX - 36, WINCY - 14 };

	m_tLevelUpText = { 5, WINCY - 370, 205, WINCY - 320 };
	m_tLevelUpButton = { 64, WINCY - 310, 146, WINCY - 266 };

	m_tQuick[0] = { 410, static_cast<LONG>(m_tInfo.vPos.y) + 36, 466, static_cast<LONG>(m_tInfo.vPos.y) + 92 };

	for(int i = 1; i < 8; ++i){
		m_tQuick[i] = { m_tQuick[i - 1].right + 2, m_tQuick[i - 1].top, m_tQuick[i - 1].right + 58, m_tQuick[i - 1].bottom };
	}

	const TEXTURE_INFO* pTexture = m_pTextureMgr->GetTextureInfo(L"UI", L"HpBar");
	m_tHpBar = { 0, 0, static_cast<LONG>(pTexture->tImgInfo.Width), static_cast<LONG>(pTexture->tImgInfo.Height) };

	pTexture = m_pTextureMgr->GetTextureInfo(L"UI", L"MpBar");
	m_tMpBar = { 0, 0, static_cast<LONG>(pTexture->tImgInfo.Width), static_cast<LONG>(pTexture->tImgInfo.Height) };

	m_tInfoRect = { 332, static_cast<LONG>(m_tInfo.vPos.y) + 102 , 954, static_cast<LONG>(m_tInfo.vPos.y) + 254 };

	return S_OK;
}

HRESULT CHUD::LateInit(){
	CObjectManager* pObjMgr = OBJMGR;

	m_pPage[STATUS] = CStatusPage::Create(m_pPlayer, this);
	if(nullptr != m_pPage[STATUS]){
		pObjMgr->AddObject(CObjectManager::OBJ_UI, m_pPage[STATUS]);
	}
	m_pPage[INVENTORY] = CInventoryPage::Create(m_pPlayer, this);
	if(nullptr != m_pPage[INVENTORY])
		pObjMgr->AddObject(CObjectManager::OBJ_UI, m_pPage[INVENTORY]);

	m_pPage[QUEST] = CQuestPage::Create(m_pPlayer);
	if(nullptr != m_pPage[QUEST])
		pObjMgr->AddObject(CObjectManager::OBJ_UI, m_pPage[QUEST]);

	m_pPage[SPELL] = CSpellPage::Create(m_pPlayer);
	if(nullptr != m_pPage[SPELL])
		pObjMgr->AddObject(CObjectManager::OBJ_UI, m_pPage[SPELL]);

	return S_OK;
}

HRESULT CHUD::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	m_tHpBar.top = static_cast<LONG>(m_tHpBar.bottom * (1.f - (static_cast<float>(m_pPlayer->m_tStat.iHp) / m_pPlayer->m_tStat.iMaxHp)));
	m_tMpBar.top = static_cast<LONG>(m_tMpBar.bottom * (1.f - (static_cast<float>(m_pPlayer->m_tStat.iMp) / m_pPlayer->m_tStat.iMaxMp)));

	D3DXVECTOR3 vMousePos = m_pKeyMgr->GetMousePos();
	int i = 0;

	if(m_bIsLevelUp && m_pPage[STATUS]->IsVisible())
		m_bIsLevelUp = false;

	if(m_wstrInfoText == L""){
		for(; i < BUTTON_END; ++i)
			if(PtInRect(&m_tButtonRect[i], POINT{ static_cast<LONG>(vMousePos.x),static_cast<LONG>(vMousePos.y) }))
				break;

		switch(PAGE_TYPE(i)){
		case CHUD::STATUS:
			m_wstrInfoText = L"CHARACTER INFORMATION\nHOT KEY: \'C\'";
			break;
		case CHUD::QUEST:
			m_wstrInfoText = L"QUEST LOG\nHOT KEY: \'Q\'";
			break;
		case CHUD::MAP:
			m_wstrInfoText = L"ATUOMAP\nHOT KEY: \'TAB\'";
			break;
		case CHUD::MENU:
			m_wstrInfoText = L"MAIN MENU\nHOT KEY: \'ESC\'";
			break;
		case CHUD::INVENTORY:
			m_wstrInfoText = L"INVENTORY\nHOT KEY: \'I\'";
			break;
		case CHUD::SPELL:
			m_wstrInfoText = L"SPELL BOOK\nHOT KEY: \'B\'";
			break;
		case CHUD::QUICK:
			m_wstrInfoText = L"SELECT CURRENT SPELL BUTTON\nHOT KEY: \'S\'";
			break;
		default:
			break;
		}


		if(i < 7 && (i == 6 || i == 2 || m_pPage[i] != nullptr)){
			if(m_pKeyMgr->IsKeyUp(KEY_LBUTTON)){
				if(i != 6 && i != 2 && m_pPage[i]->IsVisible())
					m_pPage[i]->SetVisible(false);
				else{
					switch(PAGE_TYPE(i)){
					case CHUD::STATUS:
						m_pPage[QUEST]->SetVisible(false);
						break;
					case CHUD::QUEST:
						m_pPage[STATUS]->SetVisible(false);
						break;
					case CHUD::MAP:
						m_pPlayer->m_pTerrain->SetMiniMap();
						break;
					case CHUD::MENU:
						break;
					case CHUD::INVENTORY:
						m_pPage[SPELL]->SetVisible(false);
						break;
					case CHUD::SPELL:
						m_pPage[INVENTORY]->SetVisible(false);
						break;
					case CHUD::QUICK:
						m_bIsSpellListVisible = !m_bIsSpellListVisible;
						break;
					default:
						break;
					}
					if(i != 6 && i != 2)
						m_pPage[i]->SetVisible(true);
				}
			}
		}

		if(m_wstrInfoText == L""){
			int j = 0;
			for(; j < 8; ++j){
				if(PtInRect(&m_tQuick[j], POINT{ static_cast<LONG>(vMousePos.x),static_cast<LONG>(vMousePos.y) }))
					break;
			}

			if(j < 8 && m_pQuick[j] != nullptr){
				m_wstrInfoText = m_pQuick[j]->GetStat().szName;

				if(m_pKeyMgr->IsKeyUp(KEY_LBUTTON)){
					dynamic_cast<CInventoryPage*>(m_pPage[INVENTORY])->SetSelectedItem(m_pQuick[j]);
					m_pQuick[j] = nullptr;
				} else if(m_pKeyMgr->IsKeyUp(KEY_RBUTTON)){
					m_pQuick[j]->PotionUsed();
					SafeDelete(m_pQuick[j]);
				}
			}
		}

		if(m_bIsSpellListVisible){
			bool bCheck = false;
			for(size_t i = 0; i < m_SpellList.size(); ++i){
				if(nullptr != m_pSpellRect[i]){
					if(PtInRect(m_pSpellRect[i], POINT{ static_cast<LONG>(vMousePos.x),static_cast<LONG>(vMousePos.y) })){
						bCheck = true;
						m_wstrInfoText = m_SpellList[i]->GetSpellName() + L"\nMANA: ";
						m_wstrInfoText += to_wstring(m_SpellList[i]->GetManaRequire());

						if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
							m_pPlayer->SetSpell(m_SpellList[i]);
							m_bIsSpellListVisible = false;
							break;
						} else if(m_pKeyMgr->IsKeyDown(KEY_F5)){
							m_pPlayer->SetSpellHotKey(0, m_SpellList[i]);
							m_mapSpellHotKey[L"F5"] = m_SpellList[i];
						} else if(m_pKeyMgr->IsKeyDown(KEY_F6)){
							m_pPlayer->SetSpellHotKey(1, m_SpellList[i]);
							m_mapSpellHotKey[L"F6"] = m_SpellList[i];
						} else if(m_pKeyMgr->IsKeyDown(KEY_F7)){
							m_pPlayer->SetSpellHotKey(2, m_SpellList[i]);
							m_mapSpellHotKey[L"F7"] = m_SpellList[i];
						} else if(m_pKeyMgr->IsKeyDown(KEY_F8)){
							m_pPlayer->SetSpellHotKey(3, m_SpellList[i]);
							m_mapSpellHotKey[L"F8"] = m_SpellList[i];
						}
					} 
				}
			
			}
			if(!bCheck && m_pKeyMgr->IsKeyDown(KEY_LBUTTON))
				m_bIsSpellListVisible = false;
		}
	}

	return S_OK;
}

HRESULT CHUD::LateUpdate(){
	m_pDisplaySpell = m_pPlayer->GetSpell();
	m_pPlayer->SetIsSpellChainging(m_bIsSpellListVisible);

	if(m_bIsSpellListVisible){
		dynamic_cast<CSpellPage*>(m_pPage[SPELL])->GetSpellVector(m_SpellList);
		if(m_SpellList.size() != m_pSpellRect.size())
			SpellRectCreate();
	}

	return S_OK;
}

HRESULT CHUD::Render(){
	const TEXTURE_INFO* pTextureInfo = nullptr;

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	matWorld = matScale * matTrans;

	pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);
	if(nullptr != pTextureInfo){
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);

		m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pDeviceMgr->GetSprite()->End();
	}

	pTextureInfo = m_pTextureMgr->GetTextureInfo(L"UI", L"HpBar");
	if(nullptr != pTextureInfo){
		D3DXVECTOR3 vHpBar = { 96.f, static_cast<float>(m_tHpBar.top), 0.f };
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, &m_tHpBar, &pTextureInfo->vec3CenterPoint, &vHpBar, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pDeviceMgr->GetSprite()->End();
	}

	pTextureInfo = m_pTextureMgr->GetTextureInfo(L"UI", L"MpBar");
	if(nullptr != pTextureInfo){
		D3DXVECTOR3 vMpBar = { 461.f, static_cast<float>(m_tMpBar.top), 0.f };
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
		m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, &m_tMpBar, &pTextureInfo->vec3CenterPoint, &vMpBar, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pDeviceMgr->GetSprite()->End();
	}

	for(int i = 0; i < 8; ++i){
		if(nullptr != m_pQuick[i]){
			pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Item", m_pQuick[i]->GetObjectKey(), 0, L"Inventory");
			if(nullptr != pTextureInfo){
				INIT_STRUCT(matScale);
				INIT_STRUCT(matTrans);
				D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
				D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tQuick[i].left), static_cast<float>(m_tQuick[i].top), 0.f);
				matWorld = matScale * matTrans;
				m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_pDeviceMgr->GetSprite()->End();
			}
		}
	}

	if(m_pDisplaySpell != nullptr){
		pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Spell", m_pDisplaySpell->GetIconKey());
		if(nullptr != pTextureInfo){
			INIT_STRUCT(matScale);
			INIT_STRUCT(matTrans);
			D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
			D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tButtonRect[QUICK].left), static_cast<float>(m_tButtonRect[QUICK].top), 0.f);
			matWorld = matScale * matTrans;
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pDeviceMgr->GetSprite()->End();
		}
	}

	if(m_bIsSpellListVisible){
		for(size_t i = 0; i < m_SpellList.size(); ++i)
			if(nullptr != m_SpellList[i]){
				pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Spell", m_SpellList[i]->GetIconKey());
				if(nullptr != pTextureInfo){
					INIT_STRUCT(matScale);
					INIT_STRUCT(matTrans);
					D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
					D3DXMatrixTranslation(&matTrans, static_cast<float>(m_pSpellRect[i]->left), static_cast<float>(m_pSpellRect[i]->top), 0.f);
					matWorld = matScale * matTrans;
					m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
					m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
					m_pDeviceMgr->GetSprite()->End();
				}
			}
	}




	if(m_bIsLevelUp){
		pTextureInfo = m_pTextureMgr->GetTextureInfo(L"UI", L"Button", 0, L"LevelUp");

		if(nullptr != pTextureInfo){
			INIT_STRUCT(matScale);
			INIT_STRUCT(matTrans);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND);
			D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
			D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tLevelUpButton.left), static_cast<float>(m_tLevelUpButton.top), 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pDeviceMgr->GetSprite()->End();
		}
	}
	

	INIT_STRUCT(matWorld);
	D3DXMatrixScaling(&matWorld, 1.f, 1.f, 0.f);

	m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	m_pDeviceMgr->GetBigFont()->DrawText(m_pDeviceMgr->GetSprite(), m_wstrInfoText.c_str(), -1, &m_tInfoRect, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
	m_pDeviceMgr->GetSprite()->End();

	if(m_bIsLevelUp){
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		m_pDeviceMgr->GetBigFont()->DrawText(m_pDeviceMgr->GetSprite(), L"Level Up", -1, &m_tLevelUpText, DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pDeviceMgr->GetSprite()->End();
	}

	if(m_bIsSpellListVisible){
		for(map<wstring, CSpell*>::iterator iter = m_mapSpellHotKey.begin(); iter != m_mapSpellHotKey.end(); ++iter){
			for(size_t i = 0; i < m_SpellList.size(); ++i){
				if(m_SpellList[i] == iter->second){
					m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
					m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
					m_pDeviceMgr->GetSmallFont()->DrawText(m_pDeviceMgr->GetSprite(), iter->first.c_str(), -1, m_pSpellRect[i], DT_NOCLIP | DT_TOP | DT_RIGHT, D3DCOLOR_ARGB(255, 255, 255, 255));
					m_pDeviceMgr->GetSprite()->End();
				}
			}
		}
	}

	if(m_pKeyMgr->IsKeyPressing(KEY_D)){
		for(const RECT& rc : m_tButtonRect){
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

		for(const RECT& rc : m_tQuick){
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

		const RECT& rc = m_tInfoRect;

		D3DXVECTOR2 pt[5] = { { static_cast<float>(rc.left), static_cast<float>(rc.top) },
		{ static_cast<float>(rc.right), static_cast<float>(rc.top) },
		{ static_cast<float>(rc.right), static_cast<float>(rc.bottom) },
		{ static_cast<float>(rc.left), static_cast<float>(rc.bottom) },
		{ static_cast<float>(rc.left), static_cast<float>(rc.top) }
		};

		m_pDeviceMgr->GetLine()->Begin();
		m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pDeviceMgr->GetLine()->End();

		if(m_bIsLevelUp){
			D3DXVECTOR2 LevelPt[5] = { { static_cast<float>(m_tLevelUpText.left), static_cast<float>(m_tLevelUpText.top) },
			{ static_cast<float>(m_tLevelUpText.right), static_cast<float>(m_tLevelUpText.top) },
			{ static_cast<float>(m_tLevelUpText.right), static_cast<float>(m_tLevelUpText.bottom) },
			{ static_cast<float>(m_tLevelUpText.left), static_cast<float>(m_tLevelUpText.bottom) },
			{ static_cast<float>(m_tLevelUpText.left), static_cast<float>(m_tLevelUpText.top) }
			};

			m_pDeviceMgr->GetLine()->Begin();
			m_pDeviceMgr->GetLine()->Draw(LevelPt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pDeviceMgr->GetLine()->End();

			D3DXVECTOR2 ButtonPt[5] = { { static_cast<float>(m_tLevelUpButton.left), static_cast<float>(m_tLevelUpButton.top) },
			{ static_cast<float>(m_tLevelUpButton.right), static_cast<float>(m_tLevelUpButton.top) },
			{ static_cast<float>(m_tLevelUpButton.right), static_cast<float>(m_tLevelUpButton.bottom) },
			{ static_cast<float>(m_tLevelUpButton.left), static_cast<float>(m_tLevelUpButton.bottom) },
			{ static_cast<float>(m_tLevelUpButton.left), static_cast<float>(m_tLevelUpButton.top) }
			};

			m_pDeviceMgr->GetLine()->Begin();
			m_pDeviceMgr->GetLine()->Draw(ButtonPt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pDeviceMgr->GetLine()->End();
		}

		if(m_bIsSpellListVisible){
			for(RECT* elem : m_pSpellRect){
				D3DXVECTOR2 SpellRectPt[5] = { { static_cast<float>(elem->left), static_cast<float>(elem->top) },
				{ static_cast<float>(elem->right), static_cast<float>(elem->top) },
				{ static_cast<float>(elem->right), static_cast<float>(elem->bottom) },
				{ static_cast<float>(elem->left), static_cast<float>(elem->bottom) },
				{ static_cast<float>(elem->left), static_cast<float>(elem->top) }
				};

				m_pDeviceMgr->GetLine()->Begin();
				m_pDeviceMgr->GetLine()->Draw(SpellRectPt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_pDeviceMgr->GetLine()->End();
			}
		}
	}

	return S_OK;
}

CHUD * CHUD::Create(CPlayer* _pPlayer){
	CHUD* pInstance = new CHUD;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_pPlayer = _pPlayer;

	return pInstance;
}