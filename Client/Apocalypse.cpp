#include "stdafx.h"
#include "Apocalypse.h"
#include "ExplosionEffect.h"
#include "Player.h"


CApocalypse::CApocalypse(): m_pScrollMgr(SCROLLMGR){
}


CApocalypse::~CApocalypse(){
}

HRESULT CApocalypse::Use(const D3DXVECTOR3 _vPos) const{
	for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_MONSTER)){
		D3DXVECTOR3 vPos = elem->GetInfo().vPos - m_pScrollMgr->GetScroll();

		if(vPos.x > WINCX || vPos.x < 0 ||
			vPos.y > WINCY || vPos.y < 0)
			continue;
		if(!elem->GetIsDead()){
			elem->SetDamage(20);
			CGameObject* pObj = CExplosionEffect::Create(elem->GetInfo().vPos, m_pColTile);
			if(nullptr != pObj)
				m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pObj);
		}
	}

	return S_OK;
}

CApocalypse * CApocalypse::Create(CPlayer * _pPlayer){
	CApocalypse * pInstance = new CApocalypse;
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pColTile = _pPlayer->GetColTile();

	pInstance->m_wstrAttr = L"Fire";
	pInstance->m_wstrSpellName = L"APOCALYPSE";
	pInstance->m_IconKey = L"Apocalypse";

	pInstance->m_iManaRequire = 70;

	return pInstance;
}
