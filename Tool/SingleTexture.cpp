#include "stdafx.h"
#include "SingleTexture.h"

CSingleTexture::CSingleTexture(){

}

CSingleTexture::~CSingleTexture(){
	Release();
}

void CSingleTexture::Release(){
	m_pTextureInfo->pTexture->Release();        //텍스쳐를 담고 있는 객체도 COM객체이기 때문에 Release()함수를 가지고 있다.
												//반드시 먼저 Release()를 한 뒤에 구조체를 delete하자
	SafeDelete(m_pTextureInfo);
}

HRESULT CSingleTexture::SetTextureTrans(const DWORD _TransColor, const wstring & _wstrDirectionKey/* = L""*/){
	TEXTURE_INFO temp = {};

	temp.iImageCount = m_pTextureInfo->iImageCount;
	temp.wstrFilePath = m_pTextureInfo->wstrFilePath;
	temp.vec3CenterPoint = m_pTextureInfo->vec3CenterPoint;

	m_pTextureInfo->pTexture->Release();
	SafeDelete(m_pTextureInfo);

	LoadTexture(temp.wstrFilePath, _wstrDirectionKey, temp.iImageCount, _TransColor, temp.vec3CenterPoint);
	
	return S_OK;
}

HRESULT CSingleTexture::SetTextureCenter(const D3DXVECTOR3 & _vec3Center, const wstring & _wstrDirectionKey/* = L""*/){
	m_pTextureInfo->vec3CenterPoint = _vec3Center;
	
	return E_NOTIMPL;
}

HRESULT CSingleTexture::LoadTexture(const wstring& _wstrFilePath, const wstring& _wstrDirectionKey, UINT _iImageCount, const DWORD _TransColor, const D3DXVECTOR3& _vecCenter){
	HRESULT hr = 0;
	D3DXIMAGE_INFO tImgInfo = {};
	LPDIRECT3DTEXTURE9 pTexture = nullptr;

	LPDIRECT3DDEVICE9 pDevice = DEVICEMGR->GetDevice();
	//nullptr 체크
	if(nullptr == pDevice){
		MessageBox(g_hWnd, L"CSingleTexture::LoadTexture()에서 DEVICEMGR->GetDevice()가 nullptr입니다.", L"ERROR", MB_OK);
		return E_FAIL;
	}
	
	//파일로 부터 이미지의 정보를 읽어옴
	hr = D3DXGetImageInfoFromFile(_wstrFilePath.c_str(), &tImgInfo);
	//실패 체크
	if(FAILED(hr)){
		MessageBox(g_hWnd, _wstrFilePath.c_str(), L"이미지 정보 불러오기 실패", MB_OK);
		return E_FAIL;
	}
	
	hr = D3DXCreateTextureFromFileEx(
		pDevice,				//장치
		_wstrFilePath.c_str(),	//이미지 파일 경로
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

		_TransColor,						//제거할 색상
		nullptr,				//텍스쳐 정보(위에서 사용한 tImgInfo이기 때문에 필요가 없음)
		nullptr,				//PALETTERGB의 포인터(과거에 ARGB형을 이용할때 썻던 것, 지금은 사용 안함)
		&pTexture				//LPDIRECT3DTEXTURE9형의 포인터, 실제 텍스쳐를 받기위한 변수
	);
	//실패체크
	if(FAILED(hr)){
		MessageBox(g_hWnd, _wstrFilePath.c_str(), L"이미지 불러오기 실패", MB_OK);
		return E_FAIL;
	}
	
	m_pTextureInfo = new TEXTURE_INFO;
	m_pTextureInfo->pTexture = pTexture;
	m_pTextureInfo->tImgInfo = tImgInfo;
	m_pTextureInfo->vec3CenterPoint = _vecCenter;
	m_pTextureInfo->wstrFilePath = _wstrFilePath;
	m_pTextureInfo->iImageCount = _iImageCount;

	return S_OK;
}

CSingleTexture* CSingleTexture::Create(const wstring& _wstrFilePath){
	CSingleTexture* pTextrue = new CSingleTexture;

	if(FAILED(pTextrue->LoadTexture(_wstrFilePath))){
		SafeDelete(pTextrue);
		return nullptr;
	}

	return pTextrue;
}


