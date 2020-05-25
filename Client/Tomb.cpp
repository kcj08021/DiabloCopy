#include "stdafx.h"
#include "Tomb.h"
#include "ColTile.h"
#include "Skeleton.h"

CTomb::CTomb(): m_pObjMgr(OBJMGR){
}


CTomb::~CTomb(){
}

void CTomb::UpdateRect(){
	D3DXVECTOR3 vScreenPos = m_tInfo.vPos - m_pScrollMgr->GetScroll();
	m_tRect.left = static_cast<LONG>(vScreenPos.x) - 64;
	m_tRect.top = static_cast<LONG>(vScreenPos.y) - (m_ptRectSize.y -32);

	m_tRect.right = static_cast<LONG>(vScreenPos.x) + (m_ptRectSize.x-64);
	m_tRect.bottom = static_cast<LONG>(vScreenPos.y) + 32;
}

void CTomb::Activate(){
	SNDMGR->PlaySound(L"Tomb.wav", CSoundManager::EFFECT);
	m_bIsActivated = true;
}

HRESULT CTomb::Release(){
	return S_OK;
}

HRESULT CTomb::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Object";
	m_wstrObjectKey = L"Tomb";

	m_tStat.szName = L"TOMB";

	m_ptRectSize = { 180,168 };

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);
	m_ImgCount = pTextureInfo->iImageCount;

	CTomb::UpdateRect();

	return S_OK;
}

HRESULT CTomb::LateInit(){
	size_t Index = m_pColTile->GetIndex(m_tInfo.vPos);
	m_pColTile->SetColl(m_pColTile->GetIndex(m_tInfo.vPos), false);
	m_pColTile->SetColl(Index-80, false);

	return S_OK;
}

HRESULT CTomb::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	if(!m_bIsDead)
		if(m_bIsActivated){
			CGameObject::Animation(0.1f);
			if(m_bIsImgEnd){
				m_ImgFrame= m_ImgCount - 1;
				m_bIsDead = true;
				
				if(rand() & 1){
					size_t Index = m_pColTile->GetIndex(m_tInfo.vPos);
					CSkeleton* pObj = nullptr;
					if(m_pColTile->GetTile(Index + 1) != nullptr && m_pColTile->GetTile(Index + 1)->bIsColi){
						pObj = CSkeleton::Create(m_pColTile->GetTile(Index + 1)->vPos, m_pColTile);
						if(pObj != nullptr){
							pObj->SetAmbush();
							m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
						}
					} else if(m_pColTile->GetTile(Index + 80) != nullptr &&m_pColTile->GetTile(Index + 80)->bIsColi){
						pObj = CSkeleton::Create(m_pColTile->GetTile(Index + 80)->vPos, m_pColTile);
						if(pObj != nullptr){
							pObj->SetAmbush();
							m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
						}
					} else if(m_pColTile->GetTile(Index -1) != nullptr && m_pColTile->GetTile(Index - 1)->bIsColi){
						pObj = CSkeleton::Create(m_pColTile->GetTile(Index - 1)->vPos, m_pColTile);
						if(pObj != nullptr){
							pObj->SetAmbush();
							m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
						}
					} else if(m_pColTile->GetTile(Index - 79) != nullptr && m_pColTile->GetTile(Index - 79)->bIsColi){
						pObj = CSkeleton::Create(m_pColTile->GetTile(Index - 79)->vPos, m_pColTile);
						if(pObj != nullptr){
							pObj->SetAmbush();
							m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
						}
					} else if(m_pColTile->GetTile(Index - 81) != nullptr && m_pColTile->GetTile(Index - 81)->bIsColi){
						pObj = CSkeleton::Create(m_pColTile->GetTile(Index - 81)->vPos, m_pColTile);
						if(pObj != nullptr){
							pObj->SetAmbush();
							m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
						}
					} else if(m_pColTile->GetTile(Index - 160) != nullptr && m_pColTile->GetTile(Index - 160)->bIsColi){
						pObj = CSkeleton::Create(m_pColTile->GetTile(Index - 160)->vPos, m_pColTile);
						if(pObj != nullptr){
							pObj->SetAmbush();
							m_pObjMgr->AddObject(CObjectManager::OBJ_MONSTER, pObj);
						}
					}
				}
			}
		}

	CTomb::UpdateRect();
	return S_OK;
}

HRESULT CTomb::LateUpdate(){

	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

	if((vScreen.x < -TILECX*2.f || vScreen.x >= WINCX + TILECX*2.f || vScreen.y <= -TILECY*2.f || vScreen.y >= 700 + TILECY*2.f))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;

	return S_OK;
}

HRESULT CTomb::Render(){
	if(m_bIsVisible = true){
		const TEXTURE_INFO* pTextureInfo = nullptr;

		D3DXMATRIX matScale, matTrans, matWorld;

		const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();

		if((vScreen.x < -(TILECX << 1) || vScreen.x >= WINCX + (TILECX << 1)) || vScreen.y <= -(TILECY << 1) || vScreen.y >= 700 + (TILECY << 1))
			return S_OK;

		D3DXMatrixScaling(&matScale, m_tInfo.vSize.x, m_tInfo.vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, vScreen.x, vScreen.y, 0.f);
		matWorld = matScale * matTrans;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, m_ImgFrame, m_wstrStateKey, m_wstrDirectionKey);

		if(nullptr != pTextureInfo){
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pDeviceMgr->GetSprite()->End();
		}
		if(!m_bIsDead)
			if(KEYMGR->IsKeyPressing(KEY_D)){
				D3DXVECTOR2 pt[5] = { { static_cast<float>(m_tRect.left), static_cast<float>(m_tRect.top) },
				{ static_cast<float>(m_tRect.right), static_cast<float>(m_tRect.top) },
				{ static_cast<float>(m_tRect.right), static_cast<float>(m_tRect.bottom) },
				{ static_cast<float>(m_tRect.left), static_cast<float>(m_tRect.bottom) },
				{ static_cast<float>(m_tRect.left), static_cast<float>(m_tRect.top) }
				};

				m_pDeviceMgr->GetLine()->Begin();
				m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_pDeviceMgr->GetLine()->End();
			}
	}
	return S_OK;
}

CTomb * CTomb::Create(const D3DXVECTOR3 & _vPos, CColTile * _pColTile){
	CTomb* pInstance = new CTomb;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_pColTile = _pColTile;

	return pInstance;
}
