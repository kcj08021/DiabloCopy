#include "stdafx.h"
#include "FireWallEffect.h"
#include "ColTile.h"

CFireWallEffect::CFireWallEffect():m_eDirection(HORIZONAL), m_eIsSon(NONE), m_fHoldingTime(0.f), m_iRecur(0), m_pDelayTime(0), m_pObjMgr(OBJMGR){
}


CFireWallEffect::~CFireWallEffect(){
}

HRESULT CFireWallEffect::Release(){
	return S_OK;
}

HRESULT CFireWallEffect::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Effect";
	m_wstrObjectKey = L"FireWall";
	m_wstrStateKey = L"Create";
	m_wstrDirectionKey = L"";
	return S_OK;
}

HRESULT CFireWallEffect::LateInit(){
	size_t index = m_pColTile->GetIndex(m_tInfo.vPos);
	CFireWallEffect* pObj = nullptr;
	int iRecur = --(this->m_iRecur);
	if((m_eIsSon == NONE || m_eIsSon== UP) && m_iRecur != 0){
		size_t UpIndex = m_eDirection ? index + 1 : index + (TILEX << 1);
		
		pObj = new CFireWallEffect;
		if(!FAILED(pObj->Initialize())){
			const COL_INFO* pTile = m_pColTile->GetTile(UpIndex);
			if(nullptr != pTile){
				pObj->m_tInfo.vPos = pTile->vPos;
				pObj->m_eIsSon = UP;
				pObj->m_pColTile = this->m_pColTile;
				pObj->m_eDirection = this->m_eDirection;
				pObj->m_iRecur = iRecur;

				m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pObj);
			}
		} else{
			SafeDelete(pObj);
		}
	}

	if((m_eIsSon == NONE || m_eIsSon == DOWN) && m_iRecur != 0){
		size_t DownIndex = m_eDirection ? index - 1 : index - (TILEX << 1);

		pObj = new CFireWallEffect;
		if(!FAILED(pObj->Initialize())){
			const COL_INFO* pTile = m_pColTile->GetTile(DownIndex);
			if(nullptr != pTile){
				pObj->m_tInfo.vPos = pTile->vPos;
				pObj->m_eIsSon = DOWN;
				pObj->m_pColTile = this->m_pColTile;
				pObj->m_eDirection = this->m_eDirection;
				pObj->m_iRecur = iRecur;

				m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pObj);
			}
		} else{
			SafeDelete(pObj);
		}
	}

	return S_OK;
}

HRESULT CFireWallEffect::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
	m_ImgCount = pTextureInfo->iImageCount;

	if(L"Create" == m_wstrStateKey){
		if(m_bIsImgEnd){
			m_wstrStateKey = L"Loop";
			m_pSoundMgr->PlaySound(L"FireWall_Loop.wav", CSoundManager::EFFECT);

		}
	}

	if(m_fHoldingTime > 10.f)
		return E_FAIL;

	size_t index = m_pColTile->GetIndex(m_tInfo.vPos);

	m_pDelayTime += m_pTimeMgr->GetDeltaTime();
	if(m_pDelayTime >= 0.1f){
		m_pColTile->SetDamageTile(index, 5);
		m_pDelayTime = 0.f;
	}

	m_fHoldingTime += m_pTimeMgr->GetDeltaTime();
	
	CGameObject::Animation();

	return S_OK;
}

HRESULT CFireWallEffect::LateUpdate(){
	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

	m_pColTile->SetShadow(m_tInfo.vPos, 2);

	if((vScreen.x < -TILECX*1.5f || vScreen.x >= WINCX + TILECX*1.5f || vScreen.y <= -TILECY*1.5f || vScreen.y >= 700 + TILECY*1.5f))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;

	return S_OK;
}

HRESULT CFireWallEffect::Render(){
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

CFireWallEffect * CFireWallEffect::Create(const D3DXVECTOR3 & _vPos, CColTile* _pColTile, HORI_VERT _enum){
	CFireWallEffect* pInstance = new CFireWallEffect;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_pColTile = _pColTile;
	pInstance->m_eDirection = _enum;
	pInstance->m_iRecur = 4;

	return pInstance;
}
