#include "stdafx.h"
#include "GameObject.h"

CGameObject::CGameObject(): 
	m_bIsVisible(true), m_bIsInit(false), m_bIsDead(false),
	m_wstrTypeKey(L""), m_wstrObjectKey(L""), m_wstrStateKey(L""), m_wstrDirectionKey(L""), 
	m_pTextureInfo(nullptr), m_ImgCount(0), m_ImgFrame(0), m_fDeltaTime(0.f), m_bIsImgEnd(false),
	m_pDeviceMgr(DEVICEMGR), m_pTextureMgr(TEXTUREMGR), m_pScrollMgr(SCROLLMGR), m_pTimeMgr(TIMEMGR), m_pSoundMgr(SNDMGR)
{
	INIT_STRUCT(m_tInfo);
	INIT_STRUCT(m_tRect);
	INIT_STRUCT(m_ptRectSize);
}

CGameObject::~CGameObject(){

}

void CGameObject::UpdateAngle(){
	float fDegree = 0.f;
	float fRadian = D3DXVec3Dot(&m_tInfo.vDir, &D3DXVECTOR3{ 1.f,0.f,0.f });

	if(m_tInfo.vDir.y < 0)
		fDegree = -D3DXToDegree(acosf(fRadian));
	else
		fDegree = D3DXToDegree(acosf(fRadian));

	if(22.5f >= fDegree && -22.5 < fDegree) m_wstrDirectionKey = L"Right";
	else if(-22.5 >= fDegree && -67.5f < fDegree) m_wstrDirectionKey = L"RightUp";
	else if(-67.5f >= fDegree && -112.5f < fDegree) m_wstrDirectionKey = L"Up";
	else if(112.5f >= fDegree && 67.5f < fDegree) m_wstrDirectionKey = L"Down";
	else if(67.5f >= fDegree && 22.5f < fDegree) m_wstrDirectionKey = L"RightDown";
	else if(-157.5 >= fDegree || 157.5 < fDegree) m_wstrDirectionKey = L"Left";
	else if(-112.5f >= fDegree && -157.5 < fDegree) m_wstrDirectionKey = L"LeftUp";
	else if(157.5 >= fDegree && 112.5f < fDegree) m_wstrDirectionKey = L"LeftDown";
}

void CGameObject::UpdateRect(){
	D3DXVECTOR3 vScreenPos = m_tInfo.vPos - m_pScrollMgr->GetScroll();
	m_tRect.left = static_cast<LONG>(vScreenPos.x) - (m_ptRectSize.x >> 1);
	m_tRect.top = static_cast<LONG>(vScreenPos.y) - m_ptRectSize.y;
	
	m_tRect.right = static_cast<LONG>(vScreenPos.x) + (m_ptRectSize.x >> 1);
	m_tRect.bottom = static_cast<LONG>(vScreenPos.y);
}

int CGameObject::Animation(float _fDeltaTime/* = 0.066f*/){
	int tempFrame = m_ImgFrame;

	m_fDeltaTime += m_pTimeMgr->GetDeltaTime();

	if(m_fDeltaTime > _fDeltaTime){
		++tempFrame;
		m_fDeltaTime = 0.f;
	}

	if(tempFrame == m_ImgCount)
		m_bIsImgEnd = true;
	else
		m_bIsImgEnd = false;

	m_ImgFrame = tempFrame % m_ImgCount;

	return m_ImgFrame;
}

void CGameObject::SetDamage(int _iDamage){
	if(!m_bIsDead){
		if(m_wstrStateKey != L"Die"){
			m_tStat.iHp -= _iDamage;
			if(m_tStat.iHp <= 0){
				m_tStat.iHp = 0;
				m_ImgFrame = 0;
				m_wstrStateKey = L"Die";
			} else if(m_wstrStateKey == L"Idle"){
				m_ImgFrame = 0;
				m_wstrStateKey = L"Hit";
			}
		}
	}
}

void CGameObject::SetAinimationInit(){
	m_ImgFrame = 0;
	m_bIsImgEnd = false;
}

HRESULT CGameObject::LateInit(){
	if(!m_bIsInit){
		this->LateInit();
		m_bIsInit = true;
	}

	return S_OK;
}
