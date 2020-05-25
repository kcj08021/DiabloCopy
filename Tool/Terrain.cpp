#include "stdafx.h"
#include "Terrain.h"
#include "ToolView.h"


CTerrain::CTerrain(): m_pDeviceMgr(DEVICEMGR), m_pTextureMgr(TEXTUREMGR), m_bIsTileGrid(FALSE), m_bIsDisplayIndex(FALSE){
}


CTerrain::~CTerrain(){
	Release();
}

int CTerrain::GetIndex(const D3DXVECTOR3 & _vPos) const{

	int iX = static_cast<int>(_vPos.x / TILECX + (_vPos.y + TILECY*0.5f) / TILECY);
	int iY = static_cast<int>((_vPos.y + TILECY*0.5f) / TILECY - _vPos.x / TILECX);

	if(iX >= TILEX || iY >= TILEY)
		return -1;

	int index = iY*TILEX + iX;
	//enum{ UP, RIGHT, DOWN, LEFT };

	//size_t index = 0;
	//bool bIsFindIndex = false;

	//for(; index < m_vecTile.size(); ++index){
	//	const TILE_INFO* pTile = m_vecTile[index];
	//	// 마름모의 꼭지점
	//	D3DXVECTOR3 vPoint[4] = {
	//		{ pTile->vPos.x, pTile->vPos.y - TILECY * 0.5f, 0.f },
	//		{ pTile->vPos.x + TILECX * 0.5f, pTile->vPos.y, 0.f },
	//		{ pTile->vPos.x, pTile->vPos.y + TILECY * 0.5f, 0.f },
	//		{ pTile->vPos.x - TILECX * 0.5f, pTile->vPos.y, 0.f },
	//	};

	//	// 각 모서리에 해당하는 방향벡터
	//	D3DXVECTOR3 vDirect[4] = {
	//		vPoint[RIGHT] - vPoint[UP],
	//		vPoint[DOWN] - vPoint[RIGHT],
	//		vPoint[LEFT] - vPoint[DOWN],
	//		vPoint[UP] - vPoint[LEFT],
	//	};

	//	// 각 모서리에 대한 법선 벡터
	//	D3DXVECTOR3 vNormal[4] = {};
	//	for(int i = 0; i < 4; ++i)
	//		D3DXVec3Cross(&vNormal[i], &D3DXVECTOR3(0.f, 0.f, 1.f), &vDirect[i]);

	//	// 각 점에서 마우스까지의 벡터
	//	D3DXVECTOR3 vMouse[4] = {
	//		_vPos - vPoint[UP],
	//		_vPos - vPoint[RIGHT],
	//		_vPos - vPoint[DOWN],
	//		_vPos - vPoint[LEFT]
	//	};

	//	bIsFindIndex = true;

	//	for(int i = 0; i < 4; ++i){
	//		if(0.f > D3DXVec3Dot(&vMouse[i], &vNormal[i])){
	//			bIsFindIndex = false;
	//			break;
	//		}
	//	}

	//	if(bIsFindIndex)
	//		break;
	//}

	return index;
}

void CTerrain::TilePicking(const CPoint& _pt, int _iIndex){
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
		TILECY*0.5f,
		0.f
	};

	int index = GetIndex(vMousePos);

	if(static_cast<signed>(m_vecTile.size()) > index && 0 <= index)
		m_vecTile[index]->byDrawId = _iIndex > 0 ? _iIndex : 0;
}

void CTerrain::SaveTile(const TCHAR* _szFilePath){
	HANDLE hFile = CreateFile(_szFilePath, GENERIC_WRITE, 0, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

	if(INVALID_HANDLE_VALUE == hFile)
		return;

	DWORD dwBytes = 0;

	for(auto& pTile : m_vecTile)
		WriteFile(hFile, pTile, sizeof(TILE_INFO), &dwBytes, nullptr);

	CloseHandle(hFile);
}

void CTerrain::LoadTile(const TCHAR* _szFilePath){
	HANDLE hFile = CreateFile(_szFilePath, GENERIC_READ, 0, 0,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

	if(INVALID_HANDLE_VALUE == hFile)
		return;

	if(!m_vecTile.empty()){
		for_each(m_vecTile.begin(), m_vecTile.end(), SafeDelete<TILE_INFO*>);
		m_vecTile.clear();
		m_vecTile.shrink_to_fit();
	}

	DWORD dwBytes = 0;
	TILE_INFO tInfo;
	TILE_INFO* pTile = nullptr;

	while(true){
		ReadFile(hFile, &tInfo, sizeof(TILE_INFO), &dwBytes, nullptr);

		if(0 == dwBytes)
			break;

		pTile = new TILE_INFO(tInfo);
		m_vecTile.push_back(pTile);
	}

	CloseHandle(hFile);
}

HRESULT CTerrain::Release(){
	for(TILE_INFO* elem : m_vecTile)
		SafeDelete(elem);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();

	return S_OK;
}

HRESULT CTerrain::Initialize(){
	m_vecTile.reserve(TILEX * TILEY);

	TILE_INFO* pTile = nullptr;
	float fX = 0.f, fY = 0.f;

	for(int i = 0; i < TILEY; ++i){
		for(int j = 0; j < TILEX; ++j){
			fX = (j - i)*(TILECX * 0.5f) + TILECX*TILEX*0.5f;
			fY = (j + i) * (TILECY * 0.5f) + TILECY*0.5f;

			pTile = new TILE_INFO;
			pTile->vPos = { fX,fY, static_cast<float>(j)};
			pTile->vSize = { 1.f,1.f,0.f };
			pTile->byDrawId = 4;
			pTile->byDrawOption = 0;

			m_vecTile.push_back(pTile);
		}
	}

	return S_OK;
}

HRESULT CTerrain::LateInit(){
	return S_OK;
}

HRESULT CTerrain::Update(){

	return S_OK;
}

HRESULT CTerrain::LateUpdate(){
	return S_OK;
}

HRESULT CTerrain::Render(){
	const TILE_INFO* pTile = nullptr;
	const TEXTURE_INFO* pTextureInfo = nullptr;
	TCHAR szIndex[MIN_STR];

	D3DXMATRIX matScale, matTrans, matWorld;

	for(size_t i = 0; i < m_vecTile.size(); ++i){
		pTile = m_vecTile[i];

		float fPosX = pTile->vPos.x - m_pView->GetScrollPos(0);
		float fPosY = pTile->vPos.y - m_pView->GetScrollPos(1);

		if((fPosX < -TILECX || fPosX >= WINCX + TILECX) || fPosY <= -TILECY || fPosY >= WINCY + TILECY)
			continue;

		D3DXMatrixScaling(&matScale, pTile->vSize.x, pTile->vSize.y, 0.f);
		D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 0.f);
		matWorld = matScale * matTrans;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Map", L"Tile", pTile->byDrawId);

		if(nullptr != pTextureInfo){
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
			m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

			m_pDeviceMgr->GetSprite()->End();
		}

	}

	if(m_bIsTileGrid || m_bIsDisplayIndex){
		for(size_t i = 0; i < m_vecTile.size(); ++i){
			
			pTile = m_vecTile[i];

			float fPosX = pTile->vPos.x - m_pView->GetScrollPos(0);
			float fPosY = pTile->vPos.y - m_pView->GetScrollPos(1);

			if((fPosX < -TILECX || fPosX >= WINCX + TILECX) || fPosY <= -TILECY || fPosY >= WINCY + TILECY)
				continue;

			if(m_bIsTileGrid){
				D3DXVECTOR2 pt[5] = { { pTile->vPos.x + static_cast<float>(TILECX / 2) - m_pView->GetScrollPos(0), pTile->vPos.y - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0), pTile->vPos.y + static_cast<float>(TILECY / 2) - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0) - static_cast<float>(TILECX / 2), pTile->vPos.y - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0), pTile->vPos.y - static_cast<float>(TILECY / 2) - m_pView->GetScrollPos(1) },
				{ pTile->vPos.x - m_pView->GetScrollPos(0) + static_cast<float>(TILECX / 2), pTile->vPos.y - m_pView->GetScrollPos(1) }
				};

				m_pDeviceMgr->GetLine()->Begin();
				m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 255, 255));
				m_pDeviceMgr->GetLine()->End();
			}

			if(m_bIsDisplayIndex){
				D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 0.f);
				matWorld = matScale * matTrans;

				m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
				m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

				swprintf_s(szIndex, L"%d", i);
				m_pDeviceMgr->GetFont()->DrawText(m_pDeviceMgr->GetSprite(),
					szIndex, lstrlen(szIndex), nullptr, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

				m_pDeviceMgr->GetSprite()->End();
			}
		}
	}

	return S_OK;
}

CTerrain* CTerrain::Create(CToolView* _pView){
	CTerrain* pInstance = new CTerrain;

	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}
	pInstance->m_pView = _pView;

	return pInstance;
}