#include "stdafx.h"
#include "ColTile.h"


CColTile::CColTile(): m_pDeviceMgr(DEVICEMGR), m_pScrollMgr(SCROLLMGR){
}


CColTile::~CColTile(){
	for(COL_INFO* elem : m_vecTile)
		SafeDelete(elem);
	m_vecTile.clear();
	m_vecTile.shrink_to_fit();
}

int CColTile::GetIndex(const D3DXVECTOR3 & _vPos) const{
	D3DXVECTOR3 temp = _vPos - D3DXVECTOR3{ TILECX*TILEX*0.5f, TILECY*0.25f, -0.f };

	int iX = static_cast<int>(temp.x / (TILECX*0.5f) + (temp.y + TILECY*0.25f) / (TILECY*0.5f));
	int iY = static_cast<int>((temp.y + TILECY*0.25f) / (TILECY*0.5f) - temp.x / (TILECX*0.5f));

	if(iX >= TILEX * 2 || iY >= TILEY * 2)
		return -1;

	int index = iY*TILEX * 2 + iX;

	return index;
}

//void CColTile::TilePicking(const CPoint& _pt){
//	D3DXVECTOR3 vMousePos{
//		static_cast<float>(_pt.x),
//		static_cast<float>(_pt.y),
//		0.f
//	};
//
//	vMousePos += D3DXVECTOR3{
//		static_cast<float>(vScroll.x),
//		static_cast<float>(vScroll.y),
//		0.f
//	};
//
//	vMousePos -= D3DXVECTOR3{
//		TILECX*TILEX*0.5f,
//		TILECY*0.25f,
//		0.f
//	};
//
//	int index = GetIndex(vMousePos);
//
//	if(static_cast<signed>(m_vecTile.size()) > index && 0 <= index)
//		m_vecTile[index]->bIsColi = !m_vecTile[index]->bIsColi;
//}

const COL_INFO * CColTile::GetTile(size_t _index) const{
	if(m_vecTile.size() < _index)
		return nullptr;

	return m_vecTile[_index];
}

const COL_INFO * CColTile::GetTile(const D3DXVECTOR3 & _vPos) const{
	size_t index = GetIndex(_vPos);
	if(m_vecTile.size() < index)
		return nullptr;

	return m_vecTile[index];
}

const COL_INFO * CColTile::GetTile(const float _fX, const float _fY) const{
	size_t index = GetIndex(D3DXVECTOR3(_fX, _fY, 0.f));
	if(m_vecTile.size() < index)
		return nullptr;

	return m_vecTile[index];
}

const int CColTile::GetDamage(const D3DXVECTOR3 & _vPos) const{
	const size_t index = GetIndex(_vPos);
	if(m_vecTile.size() < index)
		return 0;

	return m_vecDamageTile[index];
}

const CGameObject * CColTile::GetPreemTile(const D3DXVECTOR3 & _vPos) const{
	size_t index = GetIndex(_vPos);
	if(index >= m_vecPreem.size())
		return nullptr;

	return m_vecPreem[index];
}

BYTE CColTile::GetShadow(const D3DXVECTOR3 & _vPos){
	return 0;
}

const int CColTile::GetDamage(const size_t _index) const{
	if(m_vecTile.size() < _index)
		return 0;

	return m_vecDamageTile[_index];
}

void CColTile::SetColl(size_t _index, bool _bIsColl){
	if(_index >= m_vecTile.size())
		return;

	m_vecTile[_index]->bIsColi = _bIsColl;
}

void CColTile::SetDamageTile(size_t _index, int _iDamage){
	if(_index >= m_vecDamageTile.size())
		return;

	m_vecDamageTile[_index] = _iDamage;
}

void CColTile::SetPreemTile(const D3DXVECTOR3& _vPos, const CGameObject* _pObj){
	size_t index = GetIndex(_vPos);
	if(index >= m_vecPreem.size())
		return;

	m_vecPreem[index] = _pObj;
}

void CColTile::SetShadow(const D3DXVECTOR3 & _vPos, int _iRange){
	size_t Index = GetIndex(_vPos + D3DXVECTOR3{ 0.f,128.f,0.f });


	for(int i = -_iRange; i <= _iRange; ++i){
		for(int j = -_iRange; j <= _iRange; ++j){
			if(Index + i + (j * 80) >= m_vecShadow.size())
				continue;

			if(i == _iRange || i == -_iRange || j == -_iRange || j == _iRange){
				if(m_vecShadow[Index + i + (j * 80)] > 64)
					m_vecShadow[Index + i + (j * 80)] = 64;
			} else if(i == (_iRange - 1) || i == -(_iRange - 1) || j == -(_iRange - 1) || j == (_iRange - 1)){
				if(m_vecShadow[Index + i + (j * 80)] > 32)
					m_vecShadow[Index + i + (j * 80)] = 32;
			} else
				m_vecShadow[Index + i + (j * 80)] = 0;
		}

		m_vecShadow[Index] = 0;

	}
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

	DWORD dwBytes = 0;
	COL_INFO tInfo;

	int index = 0;

	while(true){
		ReadFile(hFile, &tInfo, sizeof(COL_INFO), &dwBytes, nullptr);

		if(0 == dwBytes)
			break;

		m_vecTile[index++]->bIsColi = tInfo.bIsColi;
	}

	CloseHandle(hFile);
}

HRESULT CColTile::Release(){
	return E_NOTIMPL;
}

HRESULT CColTile::Initialize(){
	m_vecTile.reserve((TILEX << 1) * (TILEY << 1));

	COL_INFO* pTile = nullptr;
	float fX = 0.f, fY = 0.f, fZ = 0.f;

	for(int i = 0; i < TILEY << 1; ++i){
		for(int j = 0; j < TILEX << 1; ++j){
			fX = (j - i)*(TILECX * 0.25f) + TILECX*TILEX*0.5f;
			fY = (j + i) * (TILECY * 0.25f) + TILECY*0.25f;
			fZ = static_cast<float>(i >> 1);

			pTile = new COL_INFO;
			pTile->vPos = { fX, fY, fZ };
			pTile->vSize = { 1.f,1.f,0.f };
			pTile->bIsColi = true;

			m_vecTile.push_back(pTile);
		}
	}

	m_vecDamageTile.resize(m_vecTile.size());
	m_vecPreem.resize(m_vecTile.size());
	m_vecShadow.resize(m_vecTile.size());

	for(BYTE& elem : m_vecShadow)
		elem = 128;

	return S_OK;
}

HRESULT CColTile::Update(){
	for(BYTE& elem : m_vecShadow)
		elem = 128;

	return S_OK;
}

HRESULT CColTile::LateUpdate(){
	for(int& elem : m_vecDamageTile)
		elem = 0;

	for(auto& elem : m_vecPreem)
		if(nullptr != elem)
			if(elem->GetIsDead())
				elem = nullptr;

	return S_OK;
}

HRESULT CColTile::Render(){
	const COL_INFO* pTile = nullptr;
	const TEXTURE_INFO* pTextureInfo = nullptr;
	D3DXMATRIX matScale, matTrans, matWorld;
	TCHAR szIndex[MIN_STR];

	const D3DXVECTOR3 vScroll = m_pScrollMgr->GetScroll();

	m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	for(size_t i = 0; i < m_vecTile.size(); ++i){
		pTile = m_vecTile[i];

		float fPosX = pTile->vPos.x - vScroll.x;
		float fPosY = pTile->vPos.y - vScroll.y - 128;

		if((fPosX < -(TILECX << 1) || fPosX >= WINCX + (TILECX << 1)) || fPosY <= -(TILECY << 1) || fPosY >= 700 + (TILECY << 1))
			continue;

		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, fPosX, fPosY, 0.f);
		matWorld = matScale * matTrans;

		pTextureInfo = m_pTextureMgr->GetTextureInfo(L"Map", L"Shadow");

		if(nullptr != pTextureInfo){
			m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);

			m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(m_vecShadow[i], 255, 255, 255));
		}
	}
	m_pDeviceMgr->GetSprite()->End();

	if(KEYMGR->IsKeyPressing(KEY_X)){
		for(size_t i = 0; i < m_vecTile.size(); ++i){
			if(nullptr != m_vecPreem[i])
				continue;

			pTile = m_vecTile[i];
			if(!pTile->bIsColi)
				continue;

			float fPosX = pTile->vPos.x - vScroll.x;
			float fPosY = pTile->vPos.y - vScroll.y;

			if((fPosX < -(TILECX << 1) || fPosX >= WINCX + (TILECX << 1)) || fPosY <= -(TILECY << 1) || fPosY >= 700 + (TILECY << 1))
				continue;

			D3DXVECTOR2 pt[5] = {
				{ pTile->vPos.x + static_cast<float>(TILECX >> 2) - vScroll.x, pTile->vPos.y - vScroll.y },
				{ pTile->vPos.x - vScroll.x, pTile->vPos.y + static_cast<float>(TILECY >> 2) - vScroll.y },
				{ pTile->vPos.x - vScroll.x - static_cast<float>(TILECX >> 2), pTile->vPos.y - vScroll.y },
				{ pTile->vPos.x - vScroll.x, pTile->vPos.y - static_cast<float>(TILECY >> 2) - vScroll.y },
				{ pTile->vPos.x - vScroll.x + static_cast<float>(TILECX >> 2), pTile->vPos.y - vScroll.y }
			};


			m_pDeviceMgr->GetLine()->Begin();

			m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 0, 255, 0));

			m_pDeviceMgr->GetLine()->End();

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

		for(size_t i = 0; i < m_vecTile.size(); ++i){
			if(nullptr != m_vecPreem[i])
				continue;

			pTile = m_vecTile[i];
			if(pTile->bIsColi)
				continue;

			float fPosX = pTile->vPos.x - vScroll.x;
			float fPosY = pTile->vPos.y - vScroll.y;

			if((fPosX < -TILECX || fPosX >= WINCX + TILECX) || fPosY <= -TILECY || fPosY >= WINCY + TILECY)
				continue;


			D3DXVECTOR2 pt[5] = {
				{ pTile->vPos.x + static_cast<float>(TILECX >> 2) - vScroll.x, pTile->vPos.y - vScroll.y },
				{ pTile->vPos.x - vScroll.x, pTile->vPos.y + static_cast<float>(TILECY >> 2) - vScroll.y },
				{ pTile->vPos.x - vScroll.x - static_cast<float>(TILECX >> 2), pTile->vPos.y - vScroll.y },
				{ pTile->vPos.x - vScroll.x, pTile->vPos.y - static_cast<float>(TILECY >> 2) - vScroll.y },
				{ pTile->vPos.x - vScroll.x + static_cast<float>(TILECX >> 2), pTile->vPos.y - vScroll.y }
			};

			m_pDeviceMgr->GetLine()->Begin();

			m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 255, 0, 0));

			m_pDeviceMgr->GetLine()->End();

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

		for(size_t i = 0; i < m_vecTile.size(); ++i){
			if(nullptr == m_vecPreem[i])
				continue;

			pTile = m_vecTile[i];

			float fPosX = pTile->vPos.x - vScroll.x;
			float fPosY = pTile->vPos.y - vScroll.y;

			if((fPosX < -TILECX || fPosX >= WINCX + TILECX) || fPosY <= -TILECY || fPosY >= WINCY + TILECY)
				continue;


			D3DXVECTOR2 pt[5] = {
				{ pTile->vPos.x + static_cast<float>(TILECX >> 2) - vScroll.x, pTile->vPos.y - vScroll.y },
				{ pTile->vPos.x - vScroll.x, pTile->vPos.y + static_cast<float>(TILECY >> 2) - vScroll.y },
				{ pTile->vPos.x - vScroll.x - static_cast<float>(TILECX >> 2), pTile->vPos.y - vScroll.y },
				{ pTile->vPos.x - vScroll.x, pTile->vPos.y - static_cast<float>(TILECY >> 2) - vScroll.y },
				{ pTile->vPos.x - vScroll.x + static_cast<float>(TILECX >> 2), pTile->vPos.y - vScroll.y }
			};

			m_pDeviceMgr->GetLine()->Begin();

			m_pDeviceMgr->GetLine()->Draw(pt, 5, D3DCOLOR_ARGB(255, 0, 0, 255));

			m_pDeviceMgr->GetLine()->End();

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

	return S_OK;
}

CColTile * CColTile::Create(){
	CColTile* pInstance = new CColTile;

	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
