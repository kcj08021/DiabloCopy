#include "stdafx.h"
#include "MultiTexture.h"

CMultiTexture::CMultiTexture(){

}

CMultiTexture::~CMultiTexture(){
	Release();
}

const TEXTURE_INFO* CMultiTexture::GetTextureInfo(const wstring& _wstrDirectionKey, UINT _iIndex){
	TEXTURE_MAP::iterator find_iter = m_mapTexture.find(_wstrDirectionKey);

	if(m_mapTexture.end() == find_iter)
		return nullptr;
	else{
		if(_iIndex < 0 || _iIndex >= find_iter->second.size())
			return nullptr;

		return find_iter->second[_iIndex];
	}
}

HRESULT CMultiTexture::SetTextureTrans(const DWORD _TransColor, const wstring & _wstrDirectionKey/* = L""*/){
	auto find_iter = m_mapTexture.find(_wstrDirectionKey);

	if(m_mapTexture.end() == find_iter)
		return E_FAIL;

	TEXTURE_INFO temp = {};

	temp.iImageCount = find_iter->second[0]->iImageCount;
	temp.wstrFilePath = find_iter->second[0]->wstrFilePath;
	temp.vec3CenterPoint = find_iter->second[0]->vec3CenterPoint;

	for(TEXTURE_INFO* elem : find_iter->second){
		elem->pTexture->Release();
		SafeDelete(elem);
	}
	find_iter->second.clear();
	find_iter->second.shrink_to_fit();
	m_mapTexture.erase(find_iter);

	return LoadTexture(temp.wstrFilePath, _wstrDirectionKey, temp.iImageCount, _TransColor, temp.vec3CenterPoint);
}

HRESULT CMultiTexture::SetTextureCenter(const D3DXVECTOR3& _vec3Center, const wstring & _wstrDirectionKey/* = L""*/){
	auto find_iter = m_mapTexture.find(_wstrDirectionKey);

	if(m_mapTexture.end() == find_iter)
		return E_FAIL;

	for(auto elem : find_iter->second)
		elem->vec3CenterPoint = _vec3Center;

	return S_OK;
}


void CMultiTexture::Release(){
	for(auto& mapElem : m_mapTexture){
		for(TEXTURE_INFO* elem : mapElem.second){
			elem->pTexture->Release();
			SafeDelete(elem);
		}
		mapElem.second.clear();
		mapElem.second.shrink_to_fit();
	}
	m_mapTexture.clear();
}

HRESULT CMultiTexture::LoadTexture(const wstring& _wstrFilePath, const wstring& _wstrDirectionKey, UINT _iImageCount, const DWORD _TransColor, const D3DXVECTOR3& _vecCenter){
	HRESULT hr = 0;
	D3DXIMAGE_INFO tImgInfo = {};
	LPDIRECT3DTEXTURE9 pTexture = nullptr;
	wchar_t wstrFullFilePath[128] = L"";

	LPDIRECT3DDEVICE9 pDevice = DEVICEMGR->GetDevice();
	//nullptr 체크
	if(nullptr == pDevice){
		MessageBox(g_hWnd, L"CSingleTexture::LoadTexture()에서 DEVICEMGR->GetDevice()가 nullptr입니다.", L"ERROR", MB_OK);
		return E_FAIL;
	}
	
	TEXTURE_MAP::iterator find_iter = m_mapTexture.find(_wstrDirectionKey);
	
	if(m_mapTexture.end() != find_iter){
		return E_FAIL;
	}

	for(size_t i = 0; i < _iImageCount; ++i){
		swprintf_s(wstrFullFilePath, _wstrFilePath.c_str(), i);

		//파일로 부터 이미지의 정보를 읽어옴
		hr = D3DXGetImageInfoFromFile(wstrFullFilePath, &tImgInfo);
		//실패 체크
		if(FAILED(hr)){
			MessageBox(g_hWnd, wstrFullFilePath, L"이미지 정보 불러오기 실패", MB_OK);
			return E_FAIL;
		}

		hr = D3DXCreateTextureFromFileEx(
			pDevice,				//장치
			wstrFullFilePath,	//이미지 파일 경로
			tImgInfo.Width,			//이미지 가로 크기
			tImgInfo.Height,		//이미지 세로 크기
			tImgInfo.MipLevels,		//이미지의 밉맵 레벨
			0,						//텍스쳐 사용방식(일반적인 텍스쳐로 사용할땐 0,디폴트)
			tImgInfo.Format,		//이미지의 픽셀 포맷

			D3DPOOL_MANAGED,		//메모리 사용방식
									//D3DPOOL_DEFAULT: 기본값(VRAM만 사용)
									//D3DPOOL_MANAGED: 그래픽카드의 VRAM을 사용 하며, RAM도 사용함
									//D3DPOOL_SYSTEMMEM: 시스템 메모리(RAM)
									//속도는 순서대로 속도가 빠르지만 안정성이 나쁨

			D3DX_DEFAULT,			//텍스쳐 필터링 방식
			D3DX_DEFAULT,			//밉맵 필터링 방식
									//필터링은 텍스쳐를 확대했을때 사이사이 빈 픽셀을 어떻게 채울것인지에 대한 것

			_TransColor,			//제거할 색상
			nullptr,				//텍스쳐 정보(위에서 사용한 tImgInfo이기 때문에 필요가 없음)
			nullptr,				//PALETTERGB의 포인터(과거에 ARGB형을 이용할때 썻던 것, 지금은 사용 안함)
			&pTexture				//LPDIRECT3DTEXTURE9형의 포인터, 실제 텍스쳐를 받기위한 변수
		);
		//실패체크
		if(FAILED(hr)){
			MessageBox(g_hWnd, wstrFullFilePath, L"이미지 불러오기 실패", MB_OK);
			return E_FAIL;
		}

		TEXTURE_INFO* pTextureInfo = new TEXTURE_INFO;
		pTextureInfo->pTexture = pTexture;
		pTextureInfo->tImgInfo = tImgInfo;
		pTextureInfo->vec3CenterPoint = _vecCenter;
		pTextureInfo->wstrFilePath = _wstrFilePath;
		pTextureInfo->iImageCount = _iImageCount;

		m_mapTexture[_wstrDirectionKey].push_back(pTextureInfo);
	}

	return S_OK;
}

CMultiTexture* CMultiTexture::Create(const wstring& _wstrFilePath, const wstring& _wstrDirectionKey, UINT _iImageCount, const DWORD _TransColor, const D3DXVECTOR3& _vecCenter){
	CMultiTexture* pTextrue = new CMultiTexture;

	if(FAILED(pTextrue->LoadTexture(_wstrFilePath, _wstrDirectionKey, _iImageCount, _TransColor, _vecCenter))){
		SafeDelete(pTextrue);
		return nullptr;
	}

	return pTextrue;
}

