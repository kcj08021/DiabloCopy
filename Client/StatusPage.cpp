#include "stdafx.h"
#include "StatusPage.h"
#include "HUD.h"

CStatusPage::CStatusPage(): m_pKeyMgr(KEYMGR), m_iStatPoint(0), m_bIsButtonEnabled(false){
	INIT_ARRAY(m_tStatusBox);
	INIT_ARRAY(m_tButton);
	INIT_ARRAY(m_iButtonFrame);
}


CStatusPage::~CStatusPage(){
}

void CStatusPage::TextBoxCreate(){
	//286*46
	m_tStatusBox[NAME] = { 30, 30, 316, 76 };
	m_tStatusBox[JOB] = { 326, 30, 612, 76 };

	//108*46
	m_tStatusBox[LEVEL] = { 118, 106, 226, 149 };

	//192*46	
	m_tStatusBox[EXP] = { 420, 106, 612, 152 };
	m_tStatusBox[NEXT_EXP] = { 420, 162, 612, 208 };

	//84*48
	m_tStatusBox[STRENGTH_BASE] = { 180, 276, 264, 324 };
	m_tStatusBox[STRENGTH_NOW] = { 274, 276, 358, 324 };
	m_tStatusBox[MAGIC_BASE] = { 180, 332, 264, 380 };
	m_tStatusBox[MAGIC_NOW] = { 274, 332, 358, 380 };
	m_tStatusBox[DEXTERITY_BASE] = { 180, 388, 264, 436 };
	m_tStatusBox[DEXTERITY_NOW] = { 274, 388, 358, 436 };
	m_tStatusBox[VITALITY_BASE] = { 180, 446, 264, 488 };
	m_tStatusBox[VITALITY_NOW] = { 274, 446, 358, 488 };

	m_tStatusBox[STAT_POINT] = { 180, 498, 264, 544 };

	m_tStatusBox[LIFE_BASE] = { 180, 574, 264, 622 };
	m_tStatusBox[LIFE_NOW] = { 274, 574, 358, 622 };
	m_tStatusBox[MANA_BASE] = { 180, 630, 264, 678 };
	m_tStatusBox[MANA_NOW] = { 274, 630, 358, 678 };

	//192*46
	m_tStatusBox[GOLD] = { 420, 260, 612, 306 };

	//108*46
	m_tStatusBox[ARMOR] = { 504, 332, 612, 378 };
	m_tStatusBox[TO_HIT] = { 504, 388, 612, 434 };
	m_tStatusBox[DAMAGE] = { 504, 444, 612, 486 };

	m_tStatusBox[RESIST_MAGIC] = { 504, 518, 612, 564 };
	m_tStatusBox[RESIST_FIRE] = { 504, 574, 612, 620 };
	m_tStatusBox[RESIST_LIGHTNING] = { 504, 630, 612, 676 };
}

void CStatusPage::ButtonCreate(){
	m_tButton[UP_STRENGTH] = { 274, 276, 358, 324 };
	m_tButton[UP_MAGIC] = { 274, 332, 358, 380 };
	m_tButton[UP_DEXTERITY] = { 274, 388, 358, 436 };
	m_tButton[UP_VITALITY] = { 274, 440, 358, 488 };
}

HRESULT CStatusPage::Release(){
	return S_OK;
}

HRESULT CStatusPage::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"UI";
	m_wstrObjectKey = L"StatusPage";

	TextBoxCreate();
	ButtonCreate();

	m_bIsVisible = false;

	return S_OK;
}

HRESULT CStatusPage::LateInit(){
	return S_OK;
}

HRESULT CStatusPage::Update(){
	if(m_bIsVisible){
		STAT_INFO tEquip;
		m_pHUD->GetEquipStat(&tEquip);

		m_wstrStat[NAME] = m_pPlayer->m_tStat.szName;
		m_wstrStat[JOB] = m_pPlayer->m_tStat.szJob;
		m_wstrStat[LEVEL] = to_wstring(m_pPlayer->m_tStat.iLevel);
		m_wstrStat[EXP] = to_wstring(m_pPlayer->m_tStat.iExp);
		m_wstrStat[NEXT_EXP] = to_wstring(m_pPlayer->m_tStat.iNextExp);
		m_wstrStat[STRENGTH_BASE] = to_wstring(m_pPlayer->m_tStat.iStrength);
		m_wstrStat[MAGIC_BASE] = to_wstring(m_pPlayer->m_tStat.iMagic);
		m_wstrStat[DEXTERITY_BASE] = to_wstring(m_pPlayer->m_tStat.iDexterity);
		m_wstrStat[VITALITY_BASE] = to_wstring(m_pPlayer->m_tStat.iVitality);
		m_wstrStat[STAT_POINT] = to_wstring(m_iStatPoint);
		m_wstrStat[LIFE_BASE] = to_wstring(m_pPlayer->m_tStat.iMaxHp);
		m_wstrStat[LIFE_NOW] = to_wstring(m_pPlayer->m_tStat.iHp);
		m_wstrStat[MANA_BASE] = to_wstring(m_pPlayer->m_tStat.iMaxMp);
		m_wstrStat[MANA_NOW] = to_wstring(m_pPlayer->m_tStat.iMp);
		m_wstrStat[GOLD] = to_wstring(m_pHUD->GetGold());
		m_wstrStat[ARMOR] = to_wstring(tEquip.iArmor);
		m_wstrStat[TO_HIT] = L"100%";
		m_wstrStat[DAMAGE] = to_wstring(m_pPlayer->m_tStat.iDamage+tEquip.iDamage);
		m_wstrStat[RESIST_MAGIC] = L"0%";
		m_wstrStat[RESIST_FIRE] = L"0%";
		m_wstrStat[RESIST_LIGHTNING] = L"0%";
		m_wstrStat[STRENGTH_NOW] = to_wstring(m_pPlayer->m_tStat.iStrength);
		m_wstrStat[MAGIC_NOW] = to_wstring(m_pPlayer->m_tStat.iMagic);
		m_wstrStat[DEXTERITY_NOW] = to_wstring(m_pPlayer->m_tStat.iDexterity);
		m_wstrStat[VITALITY_NOW] = to_wstring(m_pPlayer->m_tStat.iVitality);

		if(m_iStatPoint != 0)
			m_bIsButtonEnabled = true;
		else
			m_bIsButtonEnabled = false;

		POINT mousePt = { static_cast<LONG>(m_pKeyMgr->GetMousePos().x),static_cast<LONG>(m_pKeyMgr->GetMousePos().y) };
		
		if(mousePt.x > 0 && mousePt.x < WINCX >> 1 && mousePt.y > 0 && mousePt.y < WINCY - 282)
			if(m_bIsButtonEnabled){
				if(m_pKeyMgr->IsKeyUp(KEY_LBUTTON)){
					int i = 0;
					for(; i < BUTTON_END; ++i){
						if(PtInRect(&m_tButton[i], mousePt))
							break;
					}
					switch(static_cast<BUTTON>(i)){
					case UP_STRENGTH:
						m_iButtonFrame[UP_STRENGTH] = 0;
						m_pPlayer->m_tStat.iStrength += 1;
						--m_iStatPoint;
						break;
					case UP_MAGIC:
						m_iButtonFrame[UP_MAGIC] = 0;
						m_pPlayer->m_tStat.iMagic += 1;
						--m_iStatPoint;
						break;
					case UP_DEXTERITY:
						m_iButtonFrame[UP_DEXTERITY] = 0;
						m_pPlayer->m_tStat.iDexterity += 1;
						--m_iStatPoint;
						break;
					case UP_VITALITY:
						m_iButtonFrame[UP_VITALITY] = 0;
						m_pPlayer->m_tStat.iVitality += 1;
						--m_iStatPoint;
						break;
					default:
						break;
					}
					m_pPlayer->m_tStat.iDamage = 10 + (m_pPlayer->m_tStat.iStrength * m_pPlayer->m_tStat.iLevel) / 100;
					m_pPlayer->m_tStat.iMaxHp = 100 + m_pPlayer->m_tStat.iVitality;
					m_pPlayer->m_tStat.iMaxMp = 100 + m_pPlayer->m_tStat.iMagic;
				} else if(m_pKeyMgr->IsKeyPressing(KEY_LBUTTON)){
					int i = 0;
					for(; i < BUTTON_END; ++i){
						if(PtInRect(&m_tButton[i], mousePt))
							m_iButtonFrame[i] = 1;
						else
							m_iButtonFrame[i] = 0;
					}

				}
				
			}
	}
	return S_OK;
}

HRESULT CStatusPage::LateUpdate(){
	return S_OK;
}

HRESULT CStatusPage::Render(){
	if(m_bIsVisible){
		const TEXTURE_INFO* pTextureInfo = nullptr;

		D3DXMATRIX matScale, matTrans, matWorld;

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

		if(m_bIsButtonEnabled){
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
			for(int i = 0; i < BUTTON_END; ++i){
				pTextureInfo = m_pTextureMgr->GetTextureInfo(L"UI", L"Button", m_iButtonFrame[i], L"LevelUp");
				if(nullptr == pTextureInfo)
					continue;

				INIT_STRUCT(matScale);
				INIT_STRUCT(matTrans);

				D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
				D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tButton[i].left), static_cast<float>(m_tButton[i].top), 0.f);
				matWorld = matScale * matTrans;

				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
			m_pDeviceMgr->GetSprite()->End();
		}

		matWorld = {};
		D3DXMatrixScaling(&matWorld, 1.f, 1.f, 0.f);

		for(size_t i = 0; i < BOX_END; ++i){
			if(i > 20)
				if(m_bIsButtonEnabled)
					break;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
			m_pDeviceMgr->GetSmallFont()->DrawText(m_pDeviceMgr->GetSprite(), m_wstrStat[i].c_str(), -1, &m_tStatusBox[i], DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pDeviceMgr->GetSprite()->End();
		}
		if(m_pKeyMgr->IsKeyPressing(KEY_D)){
			for(const RECT& rc : m_tStatusBox){
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
	return S_OK;
}

CStatusPage* CStatusPage::Create(CPlayer* _pPlayer, CHUD* _pHUD){
	CStatusPage* pInstance = new CStatusPage;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pHUD = _pHUD;


	return pInstance;
}
