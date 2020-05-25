#include "stdafx.h"
#include "ObjectManager.h"
#include "GameObject.h"

IMPLEMENT_SINGLETON(CObjectManager);

CObjectManager::CObjectManager(){

}

CObjectManager::~CObjectManager(){
	Release();
}

OBJ_LIST& CObjectManager::GetList(const OBJ_TYPE _enum){
	return m_ObjList[_enum];
}

CGameObject * CObjectManager::GetPlayer(){
	if(m_ObjList[OBJ_PLAYER].empty())
		return nullptr;

	return m_ObjList[OBJ_PLAYER].front();
}

void CObjectManager::AddObject(OBJ_TYPE _enum, CGameObject* _pObj){
	m_ObjList[_enum].push_back(_pObj);
}

void CObjectManager::Release(){
	for(OBJ_LIST& eList : m_ObjList){
		for(CGameObject* pObj : eList){
			if(pObj)
				SafeDelete(pObj);
		}
		eList.clear();
	}
}

int CObjectManager::Update(){

	for(OBJ_LIST& eList : m_ObjList){
		auto iter = eList.begin();
		for(; iter != eList.end(); ++iter){
			if(FAILED((*iter)->Update())){
				SafeDelete(*iter);
				iter = eList.erase(iter);
			}
			if(iter == eList.end())
				break;
		}	
	}

	return 0;
}

void CObjectManager::LateUpdate(){
	for(OBJ_LIST& eList : m_ObjList){
		for(CGameObject* pObj : eList){
			pObj->LateUpdate();
		}
	}
}

void CObjectManager::Render(){
	for(int i = 0; i < OBJ_END; ++i){
		for(auto elem : m_ObjList[i]){
				switch(i){
				case CObjectManager::OBJ_BACKGROUND:
				case CObjectManager::OBJ_PLAYER:
				case CObjectManager::OBJ_MONSTER:
				case CObjectManager::OBJ_ITEM:
				case CObjectManager::OBJ_MAPOBJ:
					m_RenderList[RENDER_GAMEOBJ].push_back(elem);
					break;
				case CObjectManager::OBJ_EFFECT:
					m_RenderList[RENDER_EFFECT].push_back(elem);
					break;
				case CObjectManager::OBJ_OTHER:
					m_RenderList[RENDER_OTHER].push_back(elem);
				case CObjectManager::OBJ_UI:
					m_RenderList[RENDER_UI].push_back(elem);
					break;
				default:
					break;
				}
		}
	}
	
	for(auto& ListElem : m_RenderList){
		ListElem.sort([](CGameObject* _pSrc, CGameObject* _pDest)->bool{
			return _pSrc->GetInfo().vPos.y < _pDest->GetInfo().vPos.y;
		}
		);

		for(auto ObjElem : ListElem)
			ObjElem->Render();

		ListElem.clear();
	}
}