#include "stdafx.h"
#include "Amulet.h"


CAmulet::CAmulet(){
}


CAmulet::~CAmulet(){
	Release();
}

HRESULT CAmulet::Release(){
	return S_OK;
}

HRESULT CAmulet::Initialize(){

	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Item";
	m_wstrObjectKey = L"Amulet";
	m_wstrStateKey = L"Field";

	m_ptRectSize = { 44,28 };

	m_iWidth = 1;
	m_iHeight = 1;
	m_iIndex = rand() &1;

	m_tStat.szName = L"AMULET";

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);;
	m_ImgCount = pTextureInfo->iImageCount;

	CItem::UpdateRect();
	return S_OK;
}

HRESULT CAmulet::LateInit(){
	return S_OK;
}

HRESULT CAmulet::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	if(!m_bIsImgEnd)
		CGameObject::Animation(0.040f);

	if(m_ImgFrame == 0 && !m_bIsSoundOut){
		m_pSoundMgr->PlaySound(L"RingDrop.wav", CSoundManager::EFFECT);
		m_bIsSoundOut = true;
	}

	if(m_bIsImgEnd)
		m_ImgFrame = m_ImgCount - 1;

	CItem::UpdateRect();
	return S_OK;
}

HRESULT CAmulet::LateUpdate(){
	return S_OK;
}

HRESULT CAmulet::Render(){
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

	return S_OK;
}

CAmulet * CAmulet::Create(const D3DXVECTOR3 & _vPos /*= D3DXVECTOR3{}*/){
	CAmulet* pInstance = new CAmulet;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;

	return pInstance;
}
