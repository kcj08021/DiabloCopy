#include "stdafx.h"
#include "Teleport.h"
#include "Player.h"
#include "ColTile.h"

CTeleport::CTeleport(){
}


CTeleport::~CTeleport(){
}

HRESULT CTeleport::Use(const D3DXVECTOR3 _vPos) const{
	D3DXVECTOR3 vWorldPos = _vPos + SCROLLMGR->GetScroll();
	if(m_pColTile->GetTile(vWorldPos)->bIsColi){
		const_cast<CPlayer*>(m_pPlayer)->SetPosition(m_pColTile->GetTile(vWorldPos)->vPos);
		SNDMGR->PlaySound(L"Teleport.wav", CSoundManager::EFFECT);
	}
	return S_OK;
}

CTeleport* CTeleport::Create(CPlayer* _pPlayer){
	CTeleport* pInstance = new CTeleport;
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pColTile = _pPlayer->GetColTile();
	pInstance->m_iManaRequire = 10;
	pInstance->m_wstrAttr = L"None";
	pInstance->m_wstrSpellName = L"TELEPORT";
	pInstance->m_IconKey = L"Teleport";


	return pInstance;
}
