#include "stdafx.h"
#include "Portal.h"


CPortal::CPortal(){
}


CPortal::~CPortal(){
}

void CPortal::UpdateRect(){
	D3DXVECTOR3 vScreenPos = m_tInfo.vPos - m_pScrollMgr->GetScroll();
	m_tRect.left = static_cast<LONG>(vScreenPos.x) - (m_ptRectSize.x >> 1);
	m_tRect.top = static_cast<LONG>(vScreenPos.y) - (m_ptRectSize.y >> 1);

	m_tRect.right = static_cast<LONG>(vScreenPos.x) + (m_ptRectSize.x >> 1);
	m_tRect.bottom = static_cast<LONG>(vScreenPos.y) + (m_ptRectSize.y >> 1);
}

void CPortal::Activate(){
	SCENEMGR->SetScene(CSceneManager::SCENE_WORLD2);
}


HRESULT CPortal::Release(){
	return S_OK;
}

HRESULT CPortal::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Object";
	m_wstrStateKey = L"Portal";

	m_tStat.szName = L"DOWN TO LEVEL 2";

	m_ptRectSize = { 128,64 };

	CPortal::UpdateRect();
	return S_OK;
}

HRESULT CPortal::LateInit(){
	return S_OK;
}

HRESULT CPortal::Update(){

	UpdateRect();
	return S_OK;
}

HRESULT CPortal::LateUpdate(){
	return S_OK;
}

HRESULT CPortal::Render(){
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

CPortal * CPortal::Create(const D3DXVECTOR3 & _vPos){
	CPortal* pInstance = new CPortal;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;

	return pInstance;
}

