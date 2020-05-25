#include "stdafx.h"
#include "FirrBoltEffect.h"
#include "ColTile.h"

CFirrBoltEffect::CFirrBoltEffect(): m_pObjMgr(OBJMGR), m_pColTile(nullptr){
}


CFirrBoltEffect::~CFirrBoltEffect(){
	Release();
}

void CFirrBoltEffect::UpdateAngle(){
	float fDegree = 0.f;
	float fRadian = D3DXVec3Dot(&m_tInfo.vDir, &D3DXVECTOR3{ 1.f,0.f,0.f });

	if(m_tInfo.vDir.y < 0)
		fDegree = -D3DXToDegree(acosf(fRadian));
	else
		fDegree = D3DXToDegree(acosf(fRadian));

	if(11.25f >= fDegree && -11.25f < fDegree) m_wstrDirectionKey = L"Right";
	else if(-11.25f >= fDegree && -33.75f < fDegree) m_wstrDirectionKey = L"RightRightUp";
	else if(-33.75f >= fDegree && -56.25f < fDegree) m_wstrDirectionKey = L"RightUp";
	else if(-56.25f >= fDegree && -78.75f < fDegree) m_wstrDirectionKey = L"RightUpUp";
	else if(-78.75f >= fDegree && -101.25f < fDegree) m_wstrDirectionKey = L"Up";
	else if(-101.25f >= fDegree && -123.75f < fDegree) m_wstrDirectionKey = L"LeftUpUp";
	else if(-123.75f >= fDegree && -146.25f < fDegree) m_wstrDirectionKey = L"LeftUp";
	else if(-146.25f >= fDegree && -168.75f < fDegree) m_wstrDirectionKey = L"LeftLeftUp";
	else if(-168.75f >= fDegree || 168.75f < fDegree) m_wstrDirectionKey = L"Left";
	else if(168.75f >= fDegree && 146.25f < fDegree) m_wstrDirectionKey = L"LeftLeftDown";
	else if(146.25f >= fDegree && 123.75f < fDegree) m_wstrDirectionKey = L"LeftDown";
	else if(123.75f >= fDegree && 101.25f < fDegree) m_wstrDirectionKey = L"LeftDownDown";
	else if(101.25f >= fDegree && 78.75f < fDegree) m_wstrDirectionKey = L"Down";
	else if(78.75f >= fDegree && 56.25f < fDegree) m_wstrDirectionKey = L"RightDownDown";
	else if(56.25f >= fDegree && 33.75f < fDegree) m_wstrDirectionKey = L"RightDown";
	else if(33.75f >= fDegree && 11.25f < fDegree) m_wstrDirectionKey = L"RightRightDown";
}

HRESULT CFirrBoltEffect::Release(){
	return S_OK;
}



HRESULT CFirrBoltEffect::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Effect";
	m_wstrObjectKey = L"FireBolt";
	m_wstrStateKey = L"Shot";
	m_wstrDirectionKey = L"Down";

	//스프라이트 처리 위한 텍스쳐 갯수 정보 얻어옴
	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
	m_ImgCount = pTextureInfo->iImageCount;

	return S_OK;
}

HRESULT CFirrBoltEffect::LateInit(){
	this->UpdateAngle();
	return S_OK;
}

HRESULT CFirrBoltEffect::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	if(m_wstrStateKey == L"Shot"){
		m_tInfo.vPos += m_tInfo.vDir * 900 * m_pTimeMgr->GetDeltaTime();

		D3DXVECTOR3 vScreenPos = m_tInfo.vPos - m_pScrollMgr->GetScroll();

		for(auto elem : m_pObjMgr->GetList(CObjectManager::OBJ_MONSTER)){
			if(!elem->GetIsDead())
				if(PtInRect(&elem->GetRect(), POINT{ static_cast<LONG>(vScreenPos.x),static_cast<LONG>(vScreenPos.y) })){
					m_ImgFrame = 0;
					m_wstrStateKey = L"Hit";
					m_wstrDirectionKey = L"";
					elem->SetDamage(1);
				}
		}

		if(vScreenPos.x > WINCX + TILECX || vScreenPos.x < -TILECX ||
			vScreenPos.y > WINCY + TILECY || vScreenPos.y < -TILECY)
			return E_FAIL;

		CGameObject::Animation(0.033f);
	} else if(m_wstrStateKey == L"Hit"){
		if(0 == CGameObject::Animation(0.033f))
			return E_FAIL;
	}

	

	return S_OK;
}

HRESULT CFirrBoltEffect::LateUpdate(){
	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();
	
	m_pColTile->SetShadow(m_tInfo.vPos, 2);

	if((vScreen.x < -TILECX*1.5f || vScreen.x >= WINCX + TILECX*1.5f || vScreen.y <= -TILECY*1.5f || vScreen.y >= 700 + TILECY*1.5f))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;
	return S_OK;
}

HRESULT CFirrBoltEffect::Render(){
	if(m_bIsVisible){
		const TEXTURE_INFO* pTextureInfo = nullptr;

		D3DXMATRIX matScale, matTrans, matWorld;

		const D3DXVECTOR3 vScroll = m_pScrollMgr->GetScroll();

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - vScroll.x, m_tInfo.vPos.y - vScroll.y, 0.f);
		matWorld = matScale * matTrans;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);

		if(nullptr != pTextureInfo){
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pDeviceMgr->GetSprite()->End();
		}
	}
	return S_OK;
}

CFirrBoltEffect * CFirrBoltEffect::Create(const D3DXVECTOR3& _vPos, const D3DXVECTOR3 & _vDir, CColTile* _pColTile){
	CFirrBoltEffect* pInstance = new CFirrBoltEffect;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_tInfo.vDir = _vDir;
	pInstance->m_pColTile = _pColTile;

	return pInstance;
}
