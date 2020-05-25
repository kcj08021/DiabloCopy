#include "stdafx.h"
#include "QuestPage.h"
#include "Player.h"

CQuestPage::CQuestPage(): m_pPlayer(nullptr), m_pKeyMgr(KEYMGR){

}


CQuestPage::~CQuestPage(){
	Release();
}

HRESULT CQuestPage::Release(){
	return S_OK;
}

HRESULT CQuestPage::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"UI";
	m_wstrObjectKey = L"QuestPage";

	m_bIsVisible = false;

	return S_OK;
}

HRESULT CQuestPage::LateInit(){
	return S_OK;
}

HRESULT CQuestPage::Update(){
	return S_OK;
}

HRESULT CQuestPage::LateUpdate(){
	return S_OK;
}

HRESULT CQuestPage::Render(){
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

		//if(m_pKeyMgr->IsKeyPressing(KEY_D)){
		//	for(const RECT& rc : m_tButtonRect){
		//		D3DXVECTOR2 pt[5] = { { static_cast<float>(rc.left), static_cast<float>(rc.top) },
		//		{ static_cast<float>(rc.right), static_cast<float>(rc.top) },
		//		{ static_cast<float>(rc.right), static_cast<float>(rc.bottom) },
		//		{ static_cast<float>(rc.left), static_cast<float>(rc.bottom) },
		//		{ static_cast<float>(rc.left), static_cast<float>(rc.top) }
		//		};

		//		m_pDeviceMgr->GetLine()->Begin();
		//		m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
		//		m_pDeviceMgr->GetLine()->End();
		//	}
		//}
	}
	return S_OK;
}

CQuestPage * CQuestPage::Create(CPlayer * _pPlayer){
	CQuestPage* pInstance = new CQuestPage;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_pPlayer = _pPlayer;

	return pInstance;
}
