#include "stdafx.h"
#include "SpellPage.h"
#include "Player.h"
#include "Spell.h"
#include "FireBolt.h"
#include "Apocalypse.h"
#include "FireWall.h"
#include "Lightning.h"
#include "ChainLightning.h"
#include "Teleport.h"
#include "Healing.h"

CSpellPage::CSpellPage(): m_pPlayer(nullptr), m_pKeyMgr(KEYMGR), m_iCurrPage(0){
	INIT_ARRAY(m_tPageButton);
	INIT_ARRAY(m_tSpellList);
	INIT_ARRAY(m_pSpell);
}


CSpellPage::~CSpellPage(){
	Release();
}

void CSpellPage::ButtonCreate(){
	m_tPageButton[0] = { static_cast<LONG>(m_tInfo.vPos.x) + 18, 644, static_cast<LONG>(m_tInfo.vPos.x) + 164, 696 };
	m_tPageButton[1] = { static_cast<LONG>(m_tInfo.vPos.x) + 170, 644, static_cast<LONG>(m_tInfo.vPos.x) + 316, 696 };
	m_tPageButton[2] = { static_cast<LONG>(m_tInfo.vPos.x) + 324, 644, static_cast<LONG>(m_tInfo.vPos.x) + 470, 696 };
	m_tPageButton[3] = { static_cast<LONG>(m_tInfo.vPos.x) + 476, 644, static_cast<LONG>(m_tInfo.vPos.x) + 622, 696 };

	for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 7; ++j){
			m_tSpellList[i][j][SPELL_ICON] = { static_cast<LONG>(m_tInfo.vPos.x) + 18 , 34 + (6 + 80) * j , static_cast<LONG>(m_tInfo.vPos.x) + 98, 114 + (6 + 80) * j };
			m_tSpellList[i][j][SPELL_INFO] = { static_cast<LONG>(m_tInfo.vPos.x) + 124 , 34 + (6 + 80) * j , static_cast<LONG>(m_tInfo.vPos.x) + 620, 114 + (6 + 80) * j };
		}

}

void CSpellPage::SpellSetting(){
	m_pSpell[0][0] = CFireBolt::Create(m_pPlayer);
	//m_pSpell[0][1] = CChargedBolt::Create(m_pPlayer);
	m_pSpell[0][2] = CHealing::Create(m_pPlayer);

	m_pSpell[1][0] = CFireWall::Create(m_pPlayer);
	m_pSpell[1][2] = CLightning::Create(m_pPlayer);

	//m_pSpell[2][0] = CManaShield::Create(m_pPlayer);
	m_pSpell[2][1] = CChainLightning::Create(m_pPlayer);

	m_pSpell[3][0] = CTeleport::Create(m_pPlayer);
	m_pSpell[3][1] = CApocalypse::Create(m_pPlayer);
}

void CSpellPage::GetSpellVector(vector<CSpell*>& _vector){
	_vector.clear();
	_vector.shrink_to_fit();
	
	for(auto& elemArray : m_pSpell)
		for(auto& elem : elemArray)
			if(elem != nullptr)
				_vector.push_back(elem);
}

void CSpellPage::SetColTile(CColTile * _pColTile){
	for(auto& elemArray : m_pSpell)
		for(auto& elem : elemArray)
			if(elem != nullptr)
				elem->SetColTile(_pColTile);
}

HRESULT CSpellPage::Release(){
	for(auto& elemArray : m_pSpell)
		for(auto& elem : elemArray)
			SafeDelete(elem);

	return S_OK;
}

HRESULT CSpellPage::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { static_cast<float>(WINCX >> 1), 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"UI";
	m_wstrObjectKey = L"SpellPage";

	m_bIsVisible = false;

	ButtonCreate();

	return S_OK;
}

HRESULT CSpellPage::LateInit(){
	SpellSetting();

	return S_OK;
}

HRESULT CSpellPage::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}
	if(m_bIsVisible){
		D3DXVECTOR3 vMousePos = m_pKeyMgr->GetMousePos();

		for(int i = 0; i < 4; ++i){
			if(PtInRect(&m_tPageButton[i], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) }))
				if(m_pKeyMgr->IsKeyUp(KEY_LBUTTON)){
					m_iCurrPage = i;
					break;
				}
		}

		for(int i = 0; i < COUNT; i++){
			if(PtInRect(&m_tSpellList[m_iCurrPage][i][SPELL_ICON], POINT{ static_cast<LONG>(vMousePos.x), static_cast<LONG>(vMousePos.y) }))
				if(m_pKeyMgr->IsKeyDown(KEY_LBUTTON)){
					m_pPlayer->SetSpell(m_pSpell[m_iCurrPage][i]);
					break;
				}
		}
	}
	return S_OK;
}

HRESULT CSpellPage::LateUpdate(){
	return S_OK;
}

HRESULT CSpellPage::Render(){
	if(m_bIsVisible){
		const TEXTURE_INFO* pTextureInfo = nullptr;
		D3DXMATRIX matScale, matTrans, matWorld;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);
		if(nullptr != pTextureInfo){
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
			matWorld = matScale * matTrans;


			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pDeviceMgr->GetSprite()->End();

		}

		for(int i = 0; i < COUNT; ++i){
			if(nullptr == m_pSpell[m_iCurrPage][i])
				continue;

			INIT_STRUCT(matScale);
			INIT_STRUCT(matTrans);

			pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Spell", m_pSpell[m_iCurrPage][i]->GetIconKey());

			if(nullptr != pTextureInfo){
				m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

				D3DXMatrixScaling(&matScale, 1.4f, 1.4f, 0.f);
				D3DXMatrixTranslation(&matTrans, static_cast<float>(m_tSpellList[m_iCurrPage][i][SPELL_ICON].left), static_cast<float>(m_tSpellList[m_iCurrPage][i][SPELL_ICON].top), 0.f);
				matWorld = matScale * matTrans;

				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_pDeviceMgr->GetSprite()->End();
			}

			wstring wstrInfo = m_pSpell[m_iCurrPage][i]->GetSpellName() + L"\nMANA: ";
			wstrInfo += to_wstring(m_pSpell[m_iCurrPage][i]->GetManaRequire());

			matWorld = {};
			D3DXMatrixScaling(&matWorld, 1.f, 1.f, 0.f);

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
			m_pDeviceMgr->GetSmallFont()->DrawText(m_pDeviceMgr->GetSprite(), wstrInfo.c_str(), -1, &m_tSpellList[m_iCurrPage][i][SPELL_INFO], DT_NOCLIP | DT_VCENTER, D3DCOLOR_ARGB(255, 255, 255, 255));
			m_pDeviceMgr->GetSprite()->End();
			
		}




		if(m_pKeyMgr->IsKeyPressing(KEY_D)){
			for(const RECT& rc : m_tPageButton){
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

			for(auto& rc2dArray : m_tSpellList)
				for(auto& rcArray : rc2dArray)
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

	return S_OK;
}

CSpellPage * CSpellPage::Create(CPlayer * _pPlayer){
	CSpellPage* pInstance = new CSpellPage;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_pPlayer = _pPlayer;

	return pInstance;
}
