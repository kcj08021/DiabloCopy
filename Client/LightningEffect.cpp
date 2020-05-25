#include "stdafx.h"
#include "LightningEffect.h"
#include "ColTile.h"


CLightningEffect::CLightningEffect(): m_iRecur(0), m_bIsCreateSon(false), m_pObjMgr(OBJMGR), m_pColTile(nullptr), m_pDamagedObj(nullptr){
	INIT_STRUCT(m_tTempRect);
}


CLightningEffect::~CLightningEffect(){
	Release();
}

void CLightningEffect::UpdateRect(){
	D3DXVECTOR3 vScreenPos = m_tInfo.vPos - m_pScrollMgr->GetScroll();
	m_tRect.left = static_cast<LONG>(vScreenPos.x) - (m_ptRectSize.x >> 1);
	m_tRect.top = static_cast<LONG>(vScreenPos.y) - (m_ptRectSize.y >> 1);

	m_tRect.right = static_cast<LONG>(vScreenPos.x) + (m_ptRectSize.x >> 1);
	m_tRect.bottom = static_cast<LONG>(vScreenPos.y) + (m_ptRectSize.y >> 1);
}

HRESULT CLightningEffect::Release(){
	return S_OK;
}

HRESULT CLightningEffect::Initialize(){
	m_tInfo.vSize = { 2.f, 2.f, 0.f };
	m_tInfo.vPos = { 0.f, 0.f, 0.f };
	m_tInfo.vDir = { 0.f, 1.f, 0.f };

	m_wstrTypeKey = L"Effect";
	m_wstrObjectKey = L"Lightning";
	m_wstrStateKey = L"Big0";
	m_wstrDirectionKey = L"";
	m_ptRectSize = { 72,48 };

	const TEXTURE_INFO* pTextureInfo = m_pTextureMgr->GetTextureInfo(m_wstrTypeKey, m_wstrObjectKey, 0, m_wstrStateKey, m_wstrDirectionKey);
	m_ImgCount = pTextureInfo->iImageCount;
	
	this->UpdateRect();
	return S_OK;
}

HRESULT CLightningEffect::LateInit(){
	this->UpdateRect();

	m_vTempPos = m_tInfo.vPos;
	memcpy(&m_tTempRect, &m_tRect, sizeof(RECT));

	return S_OK;
}

HRESULT CLightningEffect::Update(){
	if(!m_bIsInit){
		CGameObject::LateInit();
	}

	m_tInfo.vPos += m_tInfo.vDir * 900 * m_pTimeMgr->GetDeltaTime();

	D3DXVECTOR3 vScreenPos = m_tInfo.vPos - m_pScrollMgr->GetScroll();

	if(vScreenPos.x > WINCX + TILECX || vScreenPos.x < -TILECX ||
		vScreenPos.y > WINCY + TILECY || vScreenPos.y < -TILECY)
		return E_FAIL;

	for(CGameObject* elem : m_pObjMgr->GetList(CObjectManager::OBJ_MONSTER)){
		RECT rc = {};
		if(0 != IntersectRect(&rc,&elem->GetRect(), &m_tRect)){
			if(elem != m_pDamagedObj){
				m_pDamagedObj = elem;
				elem->SetDamage(2);
			}
		}
	}
	

	CGameObject::Animation(0.033f);

	this->UpdateRect();

	return S_OK;
}

HRESULT CLightningEffect::LateUpdate(){
	RECT rc = {};
	if(0 == IntersectRect(&rc, &m_tRect, &m_tTempRect) && !m_bIsCreateSon && m_iRecur!=0 && m_bIsInit){
		CLightningEffect* pObj = new CLightningEffect;
		pObj->Initialize();
		pObj->m_tInfo.vPos = this->m_vTempPos;
		pObj->m_tInfo.vDir = this->m_tInfo.vDir;
		pObj->m_pColTile = this->m_pColTile;
		pObj->m_wstrStateKey = this->m_wstrStateKey;
		pObj->m_iRecur = --(this->m_iRecur);
		pObj->m_ImgFrame = this->m_ImgFrame+2;
		m_bIsCreateSon = true;

		m_pObjMgr->AddObject(CObjectManager::OBJ_EFFECT, pObj);
	}

	const D3DXVECTOR3 vScreen = m_tInfo.vPos - m_pScrollMgr->GetScroll();
	
	m_pColTile->SetShadow(m_tInfo.vPos, 2);
	
	if((vScreen.x < -TILECX*1.5f || vScreen.x >= WINCX + TILECX*1.5f || vScreen.y <= -TILECY*1.5f || vScreen.y >= 700 + TILECY*1.5f))
		m_bIsVisible = false;
	else
		m_bIsVisible = true;

	return S_OK;
}

HRESULT CLightningEffect::Render(){
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

CLightningEffect* CLightningEffect::Create(const D3DXVECTOR3 & _vPos, const D3DXVECTOR3& _vDir, CColTile* _pColTile, SKILL_TYPE eType){
	CLightningEffect* pInstance = new CLightningEffect;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_tInfo.vPos = _vPos;
	pInstance->m_tInfo.vDir = _vDir;
	pInstance->m_pColTile = _pColTile;
	pInstance->m_wstrStateKey = eType ? L"Big1" : L"Big0";
	pInstance->m_iRecur = 7;

	return pInstance;
}
