#include "stdafx.h"
#include "TrapBarrol.h"
#include "ColTile.h"
#include "Skeleton.h"


CTrapBarrol::CTrapBarrol(): m_bIsDamaged(false){
}


CTrapBarrol::~CTrapBarrol(){
}



HRESULT CTrapBarrol::Release(){
	return S_OK;
}

HRESULT CTrapBarrol::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Monster";
	m_wstrObjectKey = L"TrapBarrol";

	m_tStat.szName = L"BARROL";

	m_ptRectSize = { 48,96 };

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);
	m_ImgCount = pTextureInfo->iImageCount;

	return S_OK;
}

HRESULT CTrapBarrol::LateInit(){
	m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);

	return S_OK;
}

HRESULT CTrapBarrol::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	if(m_wstrStateKey == L"Die"){
		m_bIsDead = true;
		m_wstrStateKey = L"";
	}

	if(m_bIsDead && !m_bIsImgEnd){
		CGameObject::Animation();
		if(!m_bIsDamaged){
			m_pSoundMgr->PlaySound(L"TrapBarrol.wav", CSoundManager::EFFECT);
			int index = m_pColTile->GetIndex(m_tInfo.vPos);

			for(int i = -1; i <= 1; ++i)
				for(int j = -80; j <= 80; j += 80){
					m_pColTile->SetDamageTile(index + i + j, 20);
				}

			m_bIsDamaged = true;
		}
	}

	if(m_bIsImgEnd){
		m_ImgFrame = m_ImgCount - 1;
		m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), true);
	}

	CGameObject::UpdateRect();
	return S_OK;
}

HRESULT CTrapBarrol::LateUpdate(){
	if(m_pColTile->GetDamage(m_tInfo.vPos)){


		m_bIsDead = true;
	}

	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

	if((vScreen.x < -TILECX*2.f || vScreen.x >= WINCX + TILECX*2.f || vScreen.y <= -TILECY*2.f || vScreen.y >= 700 + TILECY*2.f))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;

	return S_OK;
}

HRESULT CTrapBarrol::Render(){
	if(m_bIsVisible = true){
		const TEXTURE_INFO* pTextureInfo = nullptr;

		D3DXMATRIX matScale, matTrans, matWorld;

		const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

		if((vScreen.x < -(TILECX << 1) || vScreen.x >= WINCX + (TILECX << 1)) || vScreen.y <= -(TILECY << 1) || vScreen.y >= 700 + (TILECY << 1))
			return S_OK;

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

CTrapBarrol * CTrapBarrol::Create(const D3DXVECTOR3 & _vPos, CColTile * _pColTile){
	CTrapBarrol* pInstance = new CTrapBarrol;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_pColTile = _pColTile;

	return pInstance;
}
