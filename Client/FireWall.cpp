#include "stdafx.h"
#include "FireWall.h"
#include "FireWallEffect.h"
#include "Player.h"
#include "ColTile.h"

CFireWall::CFireWall(): m_pScrollMgr(SCROLLMGR){
}


CFireWall::~CFireWall(){
}

HRESULT CFireWall::Use(const D3DXVECTOR3 _vPos) const{
	CGameObject* pObj = nullptr;

	D3DXVECTOR3 vDir;
	D3DXVec3Normalize(&vDir, &(_vPos - (m_pPlayer->GetInfo().vPos - m_pScrollMgr->GetScroll())));

	const COL_INFO* pTile = m_pColTile->GetTile(_vPos + m_pScrollMgr->GetScroll());
	if(nullptr == pTile)
		return E_FAIL;

	D3DXVECTOR3 vWorldPos = pTile->vPos;

	float fDegree = 0.f;
	float fRadian = D3DXVec3Dot(&vDir, &D3DXVECTOR3{ 1.f,0.f,0.f });

	if(vDir.y < 0)
		fDegree = -D3DXToDegree(acosf(fRadian));
	else
		fDegree = D3DXToDegree(acosf(fRadian));

	if((fDegree <= 0.f && fDegree > -90.f) || (90.f<= fDegree && fDegree< 180))
		pObj = CFireWallEffect::Create(vWorldPos, m_pColTile, CFireWallEffect::VERTICAL);
	else
		pObj = CFireWallEffect::Create(vWorldPos, m_pColTile, CFireWallEffect::HORIZONAL);

	if(nullptr != pObj)
		m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pObj);

	SNDMGR->PlaySound(L"FireWall_Start.wav", CSoundManager::EFFECT);

	return S_OK;
}

CFireWall * CFireWall::Create(CPlayer * _pPlayer){
	CFireWall* pInstance = new CFireWall;
	pInstance->m_pPlayer = _pPlayer;
	pInstance->m_pColTile = _pPlayer->GetColTile();
	pInstance->m_wstrAttr = L"Fire";
	pInstance->m_wstrSpellName = L"FIRE WALL";
	pInstance->m_IconKey = L"FireWall";

	pInstance->m_iManaRequire = 15;

	return pInstance;
}
