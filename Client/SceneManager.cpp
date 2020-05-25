#include "stdafx.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Title.h"
#include "World1.h"
#include "World2.h"

IMPLEMENT_SINGLETON(CSceneManager);

CSceneManager::CSceneManager()
	: m_CurrScene(nullptr), m_eCurrSceneType(SCENE_TITLE), m_ePrevSceneType(SCENE_TITLE){

}

CSceneManager::~CSceneManager(){
	Release();
}

void CSceneManager::SetScene(const SCENE_TYPE _enum){
	if(m_CurrScene)
		SafeDelete(m_CurrScene);

	switch(_enum){
	case SCENE_TITLE:
		m_CurrScene = CTitle::Create();
		break;
	case SCENE_WORLD1:
		m_CurrScene = CWorld1::Create();
		break;
	case SCENE_WORLD2:
		m_CurrScene = CWorld2::Create();
	default:
		break;
	}
}

void CSceneManager::Release(){
	if(m_CurrScene)
		SafeDelete(m_CurrScene);
}

void CSceneManager::Update(){
	if(m_CurrScene)
		m_CurrScene->Update();
}

void CSceneManager::LateUpdate() {
	if (m_CurrScene)
		m_CurrScene->LateUpdate();
}

void CSceneManager::Render(){
	if(m_CurrScene)
		m_CurrScene->Render();
}

