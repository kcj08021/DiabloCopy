#include "stdafx.h"
#include "FireBolt.h"
#include "Player.h"
#include "FirrBoltEffect.h"


CFireBolt::CFireBolt():m_pScrollMgr(SCROLLMGR){
}


CFireBolt::~CFireBolt(){
}

HRESULT CFireBolt::Use(const D3DXVECTOR3 _vPos) const{
	//MP 모자르면 E_FAIL 리턴하게끔
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSpellPos = m_pPlayer->GetInfo().vPos + D3DXVECTOR3{0.f, -50.f, 0.f};
	D3DXVec3Normalize(&vDir, &(_vPos - (vSpellPos - m_pScrollMgr->GetScroll())));

	CGameObject* pFireBolt = CFirrBoltEffect::Create(vSpellPos, vDir, m_pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pFireBolt);

	SNDMGR->PlaySound(L"FireBolt.wav", CSoundManager::EFFECT);

	return S_OK;
}

CFireBolt * CFireBolt::Create(CPlayer * _pPlayer){
	CFireBolt* pInstance = new CFireBolt;
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pColTile = _pPlayer->GetColTile();
	pInstance->m_wstrSpellName = L"FIRE BOLT";
	pInstance->m_wstrAttr = L"Fire";
	pInstance->m_IconKey = L"FireBolt";

	pInstance->m_iManaRequire = 5;

	return pInstance;
}