#include "stdafx.h"
#include "ScrollManager.h"

IMPLEMENT_SINGLETON(CScrollManager)

CScrollManager::CScrollManager(){
	INIT_STRUCT(m_vScroll);
	INIT_STRUCT(m_vMaxScroll);
}

CScrollManager::~CScrollManager(){
}

void CScrollManager::IntiScroll(){
	m_vScroll.x = 0.f;
	m_vScroll.y = 0.f;
}

void CScrollManager::CheckScroll(){

	if(m_vMaxScroll.x != 0){
		if(m_vScroll.x > m_vMaxScroll.x)
			m_vScroll.x = m_vMaxScroll.x;
		else if(m_vScroll.x < 0.f)
			m_vScroll.x = 0.f;
	}
	if(m_vMaxScroll.y != 0){
		if(m_vScroll.y > m_vMaxScroll.y)
			m_vScroll.y = m_vMaxScroll.y;
		else if(m_vScroll.y < 0.f)
			m_vScroll.y = 0.f;
	}
}
