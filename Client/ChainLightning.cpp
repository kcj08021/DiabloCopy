#include "stdafx.h"
#include "ChainLightning.h"
#include "Player.h"
#include "LightningEffect.h"


CChainLightning::CChainLightning(): m_pObjMgr(OBJMGR), m_pScrollMgr(SCROLLMGR){
}


CChainLightning::~CChainLightning(){
}

HRESULT CChainLightning::Use(const D3DXVECTOR3 _vPos) const{
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSpellPos = m_pPlayer->GetInfo().vPos + D3DXVECTOR3{ 0.f, 0.f, 0.f };
	D3DXVec3Normalize(&vDir, &(_vPos - (vSpellPos - m_pScrollMgr->GetScroll())));

	CGameObject* pLightning = CLightningEffect::Create(vSpellPos, vDir, m_pColTile, CLightningEffect::CHAIN);
	m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pLightning);



	list<CGameObject*> m_MonsterList;

	for(auto iter = m_pObjMgr->GetList(CObjectManager::OBJ_MONSTER).begin(); iter != m_pObjMgr->GetList(CObjectManager::OBJ_MONSTER).end(); ++iter){
		if(!(*iter)->GetIsDead())
			m_MonsterList.push_back(*iter);
	}

	m_MonsterList.sort([&](CGameObject* _dest, CGameObject* _src)-> bool{
		D3DXVECTOR3 vDest = _dest->GetInfo().vPos - m_pPlayer->GetInfo().vPos;
		D3DXVECTOR3 vSrc = _src->GetInfo().vPos - m_pPlayer->GetInfo().vPos;

		return D3DXVec3Length(&vDest) < D3DXVec3Length(&vSrc);
	});

	list<CGameObject*>::iterator iter = m_MonsterList.begin();
	
	if(iter != m_MonsterList.end()){
		D3DXVECTOR3 vPos = (*iter)->GetInfo().vPos;

		D3DXVec3Normalize(&vDir, &((vPos - m_pScrollMgr->GetScroll()) - (vSpellPos - m_pScrollMgr->GetScroll())));

		pLightning = CLightningEffect::Create(vSpellPos, vDir, m_pColTile, CLightningEffect::CHAIN);
		m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pLightning);
		
		++iter;

		if(iter != m_MonsterList.end()){
			vPos = (*iter)->GetInfo().vPos;

			D3DXVec3Normalize(&vDir, &((vPos - m_pScrollMgr->GetScroll()) - (vSpellPos - m_pScrollMgr->GetScroll())));

			pLightning = CLightningEffect::Create(vSpellPos, vDir, m_pColTile, CLightningEffect::CHAIN);
			m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pLightning);
		}
	}

	SNDMGR->PlaySound(L"ChainLightning.wav", CSoundManager::EFFECT);

	return S_OK;
}

CChainLightning * CChainLightning::Create(CPlayer * _pPlayer){
	CChainLightning* pInstance = new CChainLightning;
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pColTile = _pPlayer->GetColTile();

	pInstance->m_wstrAttr = L"Light";
	pInstance->m_wstrSpellName = L"CHAIN LIGHTNING";
	pInstance->m_IconKey = L"ChainLightning";
	pInstance->m_iManaRequire = 40;

	return pInstance;
}

