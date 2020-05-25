#include "stdafx.h"
#include "ExplosionEffect.h"
#include "ColTile.h"


CExplosionEffect::CExplosionEffect():m_pColTile(nullptr){
}


CExplosionEffect::~CExplosionEffect(){
}

HRESULT CExplosionEffect::Release(){
	return E_NOTIMPL;
}

HRESULT CExplosionEffect::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Effect";
	m_wstrObjectKey = L"Explosion";
	m_wstrStateKey = L"";
	m_wstrDirectionKey = L"";

	//스프라이트 처리 위한 텍스쳐 갯수 정보 얻어옴
	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
	m_ImgCount = pTextureInfo->iImageCount;

	SNDMGR->PlaySound(L"Explosion.wav", CSoundManager::EFFECT);

	return S_OK;
}

HRESULT CExplosionEffect::LateInit(){

	return S_OK;
}

HRESULT CExplosionEffect::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	CGameObject::Animation();

	if(m_bIsImgEnd){
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CExplosionEffect::LateUpdate(){
	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

	m_pColTile->SetShadow(m_tInfo.vPos, 3);

	if((vScreen.x < -TILECX*1.5f || vScreen.x >= WINCX + TILECX*1.5f || vScreen.y <= -TILECY*1.5f || vScreen.y >= 700 + TILECY*1.5f))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;

	return S_OK;
}

HRESULT CExplosionEffect::Render(){
	if(m_bIsVisible){
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
	}
	return S_OK;
}

CExplosionEffect * CExplosionEffect::Create(const D3DXVECTOR3 & _vPos, CColTile* _pColTile){
	CExplosionEffect* pInstance = new CExplosionEffect;

	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}

	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_pColTile = _pColTile;

	return pInstance;
}
