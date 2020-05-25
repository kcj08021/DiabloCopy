#include "stdafx.h"
#include "Healing.h"
#include "Player.h"


CHealing::CHealing(){
}


CHealing::~CHealing(){
}

HRESULT CHealing::Use(const D3DXVECTOR3 _vPos) const{
	if(const_cast<CPlayer*>(m_pPlayer)->GetStat().iMaxHp < (const_cast<CPlayer*>(m_pPlayer)->GetStat().iHp += 50))
		const_cast<CPlayer*>(m_pPlayer)->GetStat().iHp = const_cast<CPlayer*>(m_pPlayer)->GetStat().iMaxHp;

	SNDMGR->PlaySound(L"Healing.wav", CSoundManager::EFFECT);

	return S_OK;
}

CHealing * CHealing::Create(CPlayer * _pPlayer){
	CHealing* pInstance = new CHealing;
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pColTile = _pPlayer->GetColTile();
	pInstance->m_iManaRequire = 10;
	pInstance->m_wstrAttr = L"None";
	pInstance->m_wstrSpellName = L"HEALING";
	pInstance->m_IconKey = L"Healing";


	return pInstance;
}
