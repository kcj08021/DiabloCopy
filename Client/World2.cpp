#include "stdafx.h"
#include "World2.h"
#include "Terrain.h"
#include "ColTile.h"
#include "Player.h"
#include "Skeleton.h"
#include "SkeletonKing.h"
#include "Tomb.h"
#include "Barrol.h"
#include "TrapBarrol.h"

CWorld2::CWorld2(){
}


CWorld2::~CWorld2(){
	Release();
}

HRESULT CWorld2::Release(){
	for(int i = 0; i < CObjectManager::OBJ_END; ++i){
		auto& list = m_pObjMgr->GetList(CObjectManager::OBJ_TYPE(i));
		switch(CObjectManager::OBJ_TYPE(i)){
		case CObjectManager::OBJ_BACKGROUND:
		case CObjectManager::OBJ_MONSTER:
		case CObjectManager::OBJ_ITEM:
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

HRESULT CWorld2::Initialize(){
	CTerrain* pTerrain = CTerrain::Create();
	if(nullptr == pTerrain){
		::MessageBox(g_hWnd, L"Terrain Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}
	pTerrain->LoadTile(L"../Data/World2.mapdat");

	m_pObjMgr->AddObject(CObjectManager::OBJ_OTHER, pTerrain);

	CColTile* pColTile = CColTile::Create();
	if(nullptr == pColTile){
		::MessageBox(g_hWnd, L"Collision Tile Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}
	pColTile->LoadTile(L"../Data/World2.coldat");
	ASTARMGR->SetColTile(pColTile);

	m_pObjMgr->AddObject(CObjectManager::OBJ_OTHER, pColTile);

	CGameObject* pPlayer = m_pObjMgr->GetPlayer();
	if(nullptr == pPlayer){
		pPlayer = CPlayer::Create(pColTile->GetTile(1952)->vPos, pColTile, pTerrain);
		m_pObjMgr->AddObject(CObjectManager::OBJ_PLAYER, pPlayer);
	} else{
		pPlayer->SetPosition(pColTile->GetTile(1952)->vPos.x, pColTile->GetTile(1952)->vPos.y);
		dynamic_cast<CPlayer*>(pPlayer)->SetColTile(pColTile);
		dynamic_cast<CPlayer*>(pPlayer)->SetTerrain(pTerrain);
	}
	const INFO tInfo = pPlayer->GetInfo();


	CGameObject* pObj = nullptr;
	for(int i = 0; i < 10; i += 2){
		pObj = CSkeleton::Create(pColTile->GetTile(1922 + i * 80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(1928 + i * 80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(1940 + i * 80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(358 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(188 + i*40)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(184 + i * 40)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(180 + i * 40)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(1740 + i * 80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj); 
		pObj = CSkeleton::Create(pColTile->GetTile(215 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(1018 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(2376 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
		pObj = CSkeleton::Create(pColTile->GetTile(3888 + (i>>1))->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(3541 + i *80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(4520 + i * 80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(4527 + i * 80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(4740 + i * 80)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(3762 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(4162 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(4562 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(5362 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

		pObj = CSkeleton::Create(pColTile->GetTile(5842 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
	}
	pObj = CSkeletonKing::Create(pColTile->GetTile(562)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);

	for(int i = 0; i < 3; ++i){
		pObj = CTrapBarrol::Create(pColTile->GetTile(2351 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);


		pObj = CTrapBarrol::Create(pColTile->GetTile(2431 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);


		pObj = CTrapBarrol::Create(pColTile->GetTile(2511 + i)->vPos, pColTile);
		m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
	}
	pObj = CTomb::Create(pColTile->GetTile(2331)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(781)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(1902)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(2870)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(2859)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(3870)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	pObj = CTomb::Create(pColTile->GetTile(3859)->vPos, pColTile);
	m_pObjMgr->AddObject(CObjectManager::OBJ_MAPOBJ, pObj);

	return S_OK;
}

HRESULT CWorld2::Update(){
	m_pObjMgr->Update();


	if(KEYMGR->IsKeyDown(KEY_DOWN))
		SCENEMGR->SetScene(CSceneManager::SCENE_WORLD1);


	return S_OK;
}

HRESULT CWorld2::LateUpdate(){
	m_pObjMgr->LateUpdate();

	return S_OK;
}

HRESULT CWorld2::Render(){
	m_pObjMgr->Render();

	return S_OK;
}

CWorld2* CWorld2::Create(){
	CWorld2* pInstance = new CWorld2;
	if(FAILED(pInstance->Initialize()))
		SafeDelete(pInstance);

	return pInstance;
}