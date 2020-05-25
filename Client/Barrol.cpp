#include "stdafx.h"
#include "Barrol.h"
#include "Player.h"
#include "ColTile.h"
#include "Skeleton.h"
#include "Gold.h"
#include "HPPotion.h"
#include "MPPotion.h"
#include "HeavyArmor.h"
#include "Sword.h"
#include "Ring.h"
#include "Amulet.h"
#include "Shield.h"
#include "Helmet.h"


CBarrol::CBarrol():m_pPlayer(dynamic_cast<CPlayer*>(OBJMGR->GetPlayer())), m_bIsItemDroped(false){
}


CBarrol::~CBarrol(){
}



HRESULT CBarrol::Release(){
	return S_OK;
}

HRESULT CBarrol::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Monster";
	m_wstrObjectKey = L"Barrol";

	m_tStat.szName = L"BARROL";

	m_ptRectSize = { 48,96 };

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);
	m_ImgCount = pTextureInfo->iImageCount;

	return S_OK;
}

HRESULT CBarrol::LateInit(){
	m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);

	return S_OK;
}

HRESULT CBarrol::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}
	if(m_wstrStateKey == L"Die"){
		m_bIsDead = true;
		m_wstrStateKey = L"";
	}

	if(m_bIsDead && !m_bIsImgEnd){
		if(!m_bIsItemDroped){
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
			m_bIsItemDroped = true;
			m_pSoundMgr->PlaySound(L"Barrol.wav", CSoundManager::EFFECT);
		}

		CGameObject::Animation();
	}

	if(m_bIsImgEnd){
		m_ImgFrame = m_ImgCount - 1;
		
		m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);
	}

	CGameObject::UpdateRect();
	return S_OK;
}

HRESULT CBarrol::LateUpdate(){
	if(m_pColTile->GetDamage(m_tInfo.vPos)){
		m_bIsDead = true;
	}

	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

	if((vScreen.x < -(TILECX << 1) || vScreen.x >= WINCX + (TILECX << 1)) || vScreen.y <= -(TILECY << 1) || vScreen.y >= 700 + (TILECY << 1))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;

	return S_OK;
}

HRESULT CBarrol::Render(){
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

CBarrol * CBarrol::Create(const D3DXVECTOR3 & _vPos, CColTile * _pColTile){
	CBarrol* pInstance = new CBarrol;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_pColTile = _pColTile;

	return pInstance;
}
