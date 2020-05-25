#include "stdafx.h"
#include "Tile.h"


CTile::CTile(){
}


CTile::~CTile(){
	Release();
}

HRESULT CTile::Release(){

	return S_OK;
}

HRESULT CTile::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Map";
	m_wstrObjectKey = L"Tile";

	return S_OK;
}

HRESULT CTile::LateInit(){
	return S_OK;
}

HRESULT CTile::Update(){
	const D3DXVECTOR3 vScroll = m_pScrollMgr->GetScroll();

	float fPosX = m_tInfo.vPos.x - vScroll.x;
	float fPosY = (m_tInfo.vPos.y + 64.f) - vScroll.y;
	if(fPosX < -TILECX*2.f || fPosX > WINCX + TILECX*2.f || fPosY < -TILECY*2.f || fPosY > 700 + TILECY*2.f)
		m_bIsVisible = false;
	else
		m_bIsVisible = true;
	return S_OK;
}

HRESULT CTile::LateUpdate(){
	return S_OK;
}

HRESULT CTile::Render(){
	if(m_bIsVisible){
		const TEXTURE_INFO* pTextureInfo = nullptr;
		pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame);

		if(nullptr != pTextureInfo){
			D3DXMATRIX matScale, matTrans, matWorld;

			const D3DXVECTOR3 vScroll = m_pScrollMgr->GetScroll();

			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			float fPosX = m_tInfo.vPos.x - vScroll.x;
			float fPosY = (m_tInfo.vPos.y + 64.f) - vScroll.y;



			D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
			D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 0.f);
			matWorld = matScale * matTrans;

			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		}
		m_pDeviceMgr->GetSprite()->End();
	}
	return S_OK;
}

CTile * CTile::Create(const D3DXVECTOR3 & _vPos, int _iDrawID){
	CTile* pInstance = new CTile;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos - D3DXVECTOR3{ 0.f,64.f,0.f };
	pInstance->m_ImgFrame = _iDrawID;

	return pInstance;
}
