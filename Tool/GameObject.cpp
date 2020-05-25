#include "stdafx.h"
#include "GameObject.h"

CGameObject::CGameObject(): m_bIsVisible(true), m_pDeviceMgr(nullptr), m_pTextureMgr(nullptr){
	INIT_STRUCT(m_tInfo);
}

CGameObject::~CGameObject(){}

HRESULT CGameObject::LateInit(){
	if(!m_bIsInit){
		this->LateInit();
		m_bIsInit = true;
	}

	return S_OK;
}
