#include "stdafx.h"
#include "World1.h"
#include "Terrain.h"
#include "ColTile.h"
#include "Player.h"
#include "Zombie.h"
#include "FallenOne.h"
#include "Butcher.h"
#include "Barrol.h"
#include "TrapBarrol.h"
#include "Tomb.h"
#include "Portal.h"


CWorld1::CWorld1(): m_pScrollMgr(SCROLLMGR){
}

CWorld1::~CWorld1(){
	Release();
}

HRESULT CWorld1::Release(){
	for(int i = 0; i < CObjectManager::OBJ_END; ++i){
		auto& list = m_pObjMgr->GetList(CObjectManager::OBJ_TYPE(i));
		switch(CObjectManager::OBJ_TYPE(i)){
		case CObjectManager::OBJ_BACKGROUND:
		case CObjectManager::OBJ_MONSTER:
		case CObjectManager::OBJ_ITEM:
		case CObjectManager::OBJ_MAPOBJ:
		case CObjectManager::OBJ_EFFECT:
		case CObjectManager::OBJ_OTHER:
			for(auto& elem : list)
				SafeDelete(elem);
			list.clear();
			continue;
		default:
			continue;
		}
	}
	return S_OK;
}

HRESULT CWorld1::Initialize(){
	SNDMGR->PlayBGM(L"BGM_Stage.mp3");

	CColTile* pColTile = CColTile::Create();
	if(nullptr == pColTile){
		::MessageBox(g_hWnd, L"Collision Tile Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}
	pColTile->LoadTile(L"../Data/World1.coldat");
	ASTARMGR->SetColTile(pColTile);

	m_pObjMgr->AddObject(CObjectManager::OBJ_OTHER, pColTile);


	CTerrain* pTerrain = CTerrain::Create();
	if(nullptr == pTerrain){
		::MessageBox(g_hWnd, L"Terrain Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}
	pTerrain->LoadTile(L"../Data/World1.mapdat");

	m_pObjMgr->AddObject(CObjectManager::OBJ_OTHER, pTerrain);


	CGameObject* pPlayer = m_pObjMgr->GetPlayer();
	if(nullptr == pPlayer){
		pPlayer = CPlayer::Create(pColTile->GetTile(270)->vPos, pColTile, pTerrain);
		m_pObjMgr->AddObject(CObjectManager::OBJ_PLAYER, pPlayer);
	} else{
		pPlayer->SetPosition(pColTile->GetTile(270)->vPos.x, pColTile->GetTile(270)->vPos.y);
		dynamic_cast<CPlayer*>(pPlayer)->SetColTile(pColTile);
		dynamic_cast<CPlayer*>(pPlayer)->SetTerrain(pTerrain);
	}
	const INFO tInfo = pPlayer->GetInfo();


	CGameObject* pObj = nullptr;


	for(int i = 0; i < 10; i+=2){
		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(322 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(322 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(723 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(72 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(1202 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(1202 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(1220 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(1220 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(2100 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(2100 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(2340 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(2340 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(998 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(1479 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(212 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(212 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(941 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(941 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(1583 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(1583 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(2375 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(2375 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(5087 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(5087 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(5896 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(5896 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(1922 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(1922 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CZombie::Create(pColTile->GetTile(2482 + i)->vPos, pColTile);
		else
			pObj = CFallenOne::Create(pColTile->GetTile(2482 + i)->vPos, pColTile);

		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
	}
	
	pObj = CButcher::Create(pColTile->GetTile(5057)->vPos, pColTile);

	m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);


	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(662)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(662)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(663)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(663)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(664)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(664)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(5049)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(5049)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);



	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(2756)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(2756)->vPos, pColTile);

	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(2596)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(2596)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(2764)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(2764)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	if(rand() & 1)
		pObj = CBarrol::Create(pColTile->GetTile(2604)->vPos, pColTile);
	else
		pObj = CTrapBarrol::Create(pColTile->GetTile(2604)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	for(int i = 0; i < 3; ++i){
		pObj = CTrapBarrol::Create(pColTile->GetTile(2699+i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);


		pObj = CTrapBarrol::Create(pColTile->GetTile(2779+i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);


		pObj = CTrapBarrol::Create(pColTile->GetTile(2859+i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
	

		if(rand() & 1)
			pObj = CBarrol::Create(pColTile->GetTile(4129 + i)->vPos, pColTile);
		else
			pObj = CTrapBarrol::Create(pColTile->GetTile(4129 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj); 

		if(rand() & 1)
			pObj = CBarrol::Create(pColTile->GetTile(4209 + i)->vPos, pColTile);
		else
			pObj = CTrapBarrol::Create(pColTile->GetTile(4209 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CBarrol::Create(pColTile->GetTile(4289 + i)->vPos, pColTile);
		else
			pObj = CTrapBarrol::Create(pColTile->GetTile(4289 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CBarrol::Create(pColTile->GetTile(4351 + i)->vPos, pColTile);
		else
			pObj = CTrapBarrol::Create(pColTile->GetTile(4351 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CBarrol::Create(pColTile->GetTile(4431 + i)->vPos, pColTile);
		else
			pObj = CTrapBarrol::Create(pColTile->GetTile(4431 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		if(rand() & 1)
			pObj = CBarrol::Create(pColTile->GetTile(4511 + i)->vPos, pColTile);
		else
			pObj = CTrapBarrol::Create(pColTile->GetTile(4511 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
	}

	pObj = CTomb::Create(pColTile->GetTile(567)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(1239)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CPortal::Create(pColTile->GetTile(5315)->vPos);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(5574)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(2405)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(2410)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(1246)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);


	m_pScrollMgr->IntiScroll();
	m_pScrollMgr->SetScroll(tInfo.vPos.x - WINCX*0.5f, tInfo.vPos.y - (WINCY - 256)*0.5f - 60);



	return S_OK;
}

HRESULT CWorld1::Update(){
	//if(m_pTerrain)
	//	m_pTerrain->Update();

	//if(m_pColTile)
	//	m_pColTile->Update();

	//if(m_pPlayer)
	//	m_pPlayer->Update();

	m_pObjMgr->Update();

	if(KEYMGR->IsKeyDown(KEY_UP))
		SCENEMGR->SetScene(CSceneManager::SCENE_WORLD2);

	return S_OK;
}

HRESULT CWorld1::LateUpdate(){
	//if(m_pTerrain)
	//	m_pTerrain->LateUpdate();

	//if(m_pColTile)
	//	m_pColTile->LateUpdate();

	//if(m_pPlayer)
	//	m_pPlayer->LateUpdate();

	m_pObjMgr->LateUpdate();

	return S_OK;
}

HRESULT CWorld1::Render(){
	//if(m_pTerrain)
	//	m_pTerrain->Render();

	//if(m_pColTile)
	//	m_pColTile->Render();

	//if(m_pPlayer)
	//	m_pPlayer->Render();

	m_pObjMgr->Render();


	return S_OK;
}

CWorld1* CWorld1::Create(){
	CWorld1* pInstance = new CWorld1;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
