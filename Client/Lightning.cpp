#include "stdafx.h"
#include "Player.h"
#include "Lightning.h"
#include "LightningEffect.h"


CLightning::CLightning():m_pScrollMgr(SCROLLMGR){
}


CLightning::~CLightning(){
}

HRESULT CLightning::Use(const D3DXVECTOR3 _vPos) const{
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSpellPos = m_pPlayer->GetInfo().vPos + D3DXVECTOR3{ 0.f, 0.f, 0.f };
	D3DXVec3Normalize(&vDir, &(_vPos - (vSpellPos - m_pScrollMgr->GetScroll())));

	CGameObject* pLightning = CLightningEffect::Create(vSpellPos, vDir, m_pColTile, CLightningEffect::NORMAL);
	m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pLightning);


	SNDMGR->PlaySound(L"Lightning.wav", CSoundManager::EFFECT);

	return S_OK;
}

CLightning * CLightning::Create(CPlayer * _pPlayer){
	CLightning* pInstance = new CLightning;
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pColTile = _pPlayer->GetColTile();

	pInstance->m_wstrAttr = L"Light";
	pInstance->m_wstrSpellName = L"LIGHTNING";
	pInstance->m_IconKey = L"Lightning";

	pInstance->m_iManaRequire = 20;

	return pInstance;
}
