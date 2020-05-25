#include "stdafx.h"
#include "ColTile.h"
#include "ToolView.h"


CColTile::CColTile(): m_pDeviceMgr(DEVICEMGR), m_pView(nullptr), m_bIsColGird(FALSE), m_bIsDisplayIndex(FALSE){
}


CColTile::~CColTile(){
	for(COL_INFO* elem : m_vecTile)
		SafeDelete(elem);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

int CColTile::GetIndex(D3DXVECTOR3 & _vPos){
		int iX = static_cast<int>(_vPos.x / (TILECX*0.5f) + (_vPos.y + TILECY*0.25f) / (TILECY*0.5f));
		int iY = static_cast<int>((_vPos.y + TILECY*0.25f) / (TILECY*0.5f) - _vPos.x / (TILECX*0.5f));

		if(iX >= TILEX<<1 || iY >= TILEY << 1)
			return -1;

		int index = iY*TILEX*2 + iX;

		return index;
}

void CColTile::TilePicking(const CPoint& _pt){
	D3DXVECTOR3 vMousePos{
		static_cast<float>(_pt.x),
		static_cast<float>(_pt.y),
		0.f
	};

	vMousePos += D3DXVECTOR3{
		static_cast<float>(m_pView->GetScrollPos(0)),
		static_cast<float>(m_pView->GetScrollPos(1)),
		0.f
	};

	vMousePos -= D3DXVECTOR3{
		TILECX*TILEX*0.5f,
		TILECY*0.25f,
		0.f
	};

	int index = GetIndex(vMousePos);

	if(static_cast<signed>(m_vecTile.size()) > index && 0 <= index)
		m_vecTile[index]->bIsColi = !m_vecTile[index]->bIsColi;
}

void CColTile::SaveTile(const TCHAR * _szFilePath){
	HANDLE hFile = CreateFile(_szFilePath, GENERIC_WRITE, 0, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if(INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwBytes = 0;

	for(auto& pTile : m_vecTile)
		WriteFile(hFile, pTile, sizeof(COL_INFO), &dwBytes, nullptr);

	CloseHandle(hFile);
}

void CColTile::LoadTile(const TCHAR * _szFilePath){
	HANDLE hFile = CreateFile(_szFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if(INVALID_HANDLE_VALUE == hFile)
		return;

	if(!m_vecTile.empty()){
		for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<COL_INFO*>);
		m_vecTile.clear();
		m_vecTile.shrink_to_fit();
	}

	DWORD dwBytes = 0;
	COL_INFO tInfo;
	COL_INFO* pTile = nullptr;

	while(true){
		ReadFile(hFile, &tInfo, sizeof(COL_INFO), &dwBytes, nullptr);

		if(0 == dwBytes)
			break;

		pTile = new COL_INFO(tInfo);
		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);
}

HRESULT CColTile::Release(){
	return E_NOTIMPL;
}

HRESULT CColTile::Initialize(){
	m_vecTile.reserve((TILEX << 1) * (TILEY << 1));

	COL_INFO* pTile = nullptr;
	float fX = 0.f, fY = 0.f;

	for(int i = 0; i < TILEY << 1; ++i){
		for(int j = 0; j < TILEX << 1; ++j){
			fX = (j - i)*(TILECX * 0.25f) + TILECX*TILEX*0.5f;
			fY = (j + i) * (TILECY * 0.25f) + TILECY*0.25f;

			pTile = new COL_INFO;
			pTile->vPos = { fX,fY,0.f };
			pTile->vSize = { 1.f,1.f,0.f };
			pTile->bIsColi = true;
			
			m_vecTile.push_back(pTile);
		}
	}
	return S_OK;
}

HRESULT CColTile::Update(){
	return E_NOTIMPL;
}

HRESULT CColTile::LateUpdate(){
	return E_NOTIMPL;
}

HRESULT CColTile::Render(){
	const COL_INFO* pTile = nullptr;
	TCHAR szIndex[MIN_STR];

	for(size_t i = 0; i < m_vecTile.size(); ++i){
		pTile = m_vecTile[i];

		float fPosX = pTile->vPos.x - m_pView->GetScrollPos(0);
		float fPosY = pTile->vPos.y - m_pView->GetScrollPos(1);

		if((fPosX < -TILECX || fPosX >= WINCX + TILECX) || fPosY <= -TILECY || fPosY >= WINCY + TILECY)
			continue;

		if(m_bIsColGird){
			D3DXVECTOR2 pt[5] = {
				{ pTile->vPos.x + static_cast<float>(TILECX >> 2) - m_pView->GetScrollPos(0), pTile->vPos.y - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0), pTile->vPos.y + static_cast<float>(TILECY >> 2) - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0) - static_cast<float>(TILECX >> 2), pTile->vPos.y - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0), pTile->vPos.y - static_cast<float>(TILECY >> 2) - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0) + static_cast<float>(TILECX >> 2), pTile->vPos.y - m_pView->GetScrollPos(1) }
			};


			m_pDeviceMgr->GetLine()->Begin();

			if(pTile->bIsColi)
				m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

			m_pDeviceMgr->GetLine()->End();

		}

		if(m_bIsDisplayIndex){
			D3DXMATRIX matTrans = {};

			D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 0.f);
			D3DXMATRIX matWorld = matTrans;
			m_pDeviceMgr->GetSprite()->SetTransform(&matTrans);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			swprintf_s(szIndex, L"%d", i);
			m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(),
				szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pDeviceMgr->GetSprite()->End();

		}

	}

	for(size_t i = 0; i < m_vecTile.size(); ++i){
		pTile = m_vecTile[i];

		float fPosX = pTile->vPos.x - m_pView->GetScrollPos(0);
		float fPosY = pTile->vPos.y - m_pView->GetScrollPos(1);

		if((fPosX < -TILECX || fPosX >= WINCX + TILECX) || fPosY <= -TILECY || fPosY >= WINCY + TILECY)
			continue;

		if(m_bIsColGird){
			D3DXVECTOR2 pt[5] = {
				{ pTile->vPos.x + static_cast<float>(TILECX >> 2) - m_pView->GetScrollPos(0), pTile->vPos.y - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0), pTile->vPos.y + static_cast<float>(TILECY >> 2) - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0) - static_cast<float>(TILECX >> 2), pTile->vPos.y - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0), pTile->vPos.y - static_cast<float>(TILECY >> 2) - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0) + static_cast<float>(TILECX >> 2), pTile->vPos.y - m_pView->GetScrollPos(1) }
			};


			m_pDeviceMgr->GetLine()->Begin();

			if(!pTile->bIsColi)
				m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 0, 0));

			m_pDeviceMgr->GetLine()->End();
		}
	}

	return S_OK;
}

CColTile * CColTile::Create(CToolView * _pView){
	CColTile* pInstance = new CColTile;

	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_pView = _pView;

	return pInstance;
}
