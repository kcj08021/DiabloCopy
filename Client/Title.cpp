#include "stdafx.h"
#include "Title.h"


CTitle::CTitle(): m_pDeviceMgr(DEVICEMGR), m_pTextureMgr(TEXTUREMGR), m_pTimeMgr(TIMEMGR), m_pKeyMgr(KEYMGR),
m_iImgCount(15), m_iImgFrame(0), m_fDeltaTime(0.f), m_iFadeInValue(0), m_bIsLoadingEnd(false), m_iFadeInOutValue(255), m_fCosine(0.f){
	INIT_STRUCT(m_tRect);
}


CTitle::~CTitle(){
	Release();
}

HRESULT CTitle::Release(){
	CloseHandle(m_LoadingThread);
	DeleteCriticalSection(&m_CriticalSection);
	SNDMGR->StopSound(CSoundManager::BGM);

	return S_OK;
}

HRESULT CTitle::Initialize(){
	HRESULT hr = 0;
	hr = m_pTextureMgr->LoadTexture(CTextureManager::SINGLE_TEXTURE, L"../Texture/UI/Title/Title.png", L"UI", L"Title");
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"Title Image Load Failed", L"Error", MB_OK);
		DestroyWindow(g_hWnd);
	}

	hr = m_pTextureMgr->LoadTexture(CTextureManager::MULTI_TEXTURE, L"../Texture/UI/Logo/Logo00%d.png", L"UI", L"Logo", 15, D3DCOLOR_ARGB(255, 0, 255, 0));
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"Title Logo Load Failed", L"Error", MB_OK);
		DestroyWindow(g_hWnd);
	}

	m_tRect = { 90, WINCY - 120, WINCX - 80, WINCY - 160 };

	m_LoadingThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, Loading, this, 0, nullptr));
	if(nullptr == m_LoadingThread){
		::MessageBox(g_hWnd, L"Thread Init Failed", L"Error", MB_OK);
		(g_hWnd);
	}

	wstrLoadingEnd = L"PRESS ENTER KEY TO CONTINUE";

	SNDMGR->PlayBGM(L"BGM_Intro.mp3");

	InitializeCriticalSection(&m_CriticalSection);

	return S_OK;
}

HRESULT CTitle::Update(){

	m_fDeltaTime += m_pTimeMgr->GetDeltaTime();

	if(m_fDeltaTime > 0.066f){
		++m_iImgFrame;

		m_fDeltaTime = 0.f;
	}

	m_iFadeInOutValue = static_cast<byte>(255.f * fabs(cosf(m_fCosine += D3DXToRadian(1.f))));

	if(m_iFadeInValue < 255)
		m_iFadeInValue += 5;
	else
		m_iFadeInValue = 255;


	m_iImgFrame = m_iImgFrame % m_iImgCount;

	DWORD dwResult = ULONG_MAX;
	GetExitCodeThread(m_LoadingThread, &dwResult);

	if(dwResult == 0)
		m_bIsLoadingEnd = true;

	if(m_bIsLoadingEnd && m_pKeyMgr->IsKeyDown(KEY_ENTER)){
		WaitForSingleObject(m_LoadingThread, INFINITE);

		SCENEMGR->SetScene(CSceneManager::SCENE_WORLD1);

		return S_OK;
	}

	return S_OK;
}

HRESULT CTitle::LateUpdate(){
	return S_OK;
}

HRESULT CTitle::Render(){
	const TEXTURE_INFO* pTextureInfo = nullptr;

	D3DXMATRIX matScale, matTrans, matWorld;

	D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
	D3DXMatrixTranslation(&matTrans, 0.f, 0.f, 0.f);
	matWorld = matScale * matTrans;

	pTextureInfo = m_pTextureMgr->GetTextureInfo(L"UI", L"Title");

	if(nullptr != pTextureInfo){
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, m_iFadeInValue, m_iFadeInValue, m_iFadeInValue));

		m_pDeviceMgr->GetSprite()->End();
	}

	INIT_STRUCT(matTrans);
	D3DXMatrixTranslation(&matTrans, 90.f, 368.f, 0.f);

	matWorld = matScale * matTrans;
	pTextureInfo = m_pTextureMgr->GetTextureInfo(L"UI", L"Logo", m_iImgFrame);

	if(nullptr != pTextureInfo){
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

		m_pDeviceMgr->GetSprite()->Draw(pTextureInfo->pTexture, nullptr, &pTextureInfo->vec3CenterPoint, nullptr, D3DCOLOR_ARGB(255, m_iFadeInValue, m_iFadeInValue, m_iFadeInValue));

		m_pDeviceMgr->GetSprite()->End();
	}

	if(m_bIsLoadingEnd){
		D3DXMatrixScaling(&matWorld, 1.f, 1.f, 0.f);
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		m_pDeviceMgr->GetBigFont()->DrawText(m_pDeviceMgr->GetSprite(), wstrLoadingEnd.c_str(), -1, &m_tRect , DT_NOCLIP | DT_CENTER | DT_VCENTER, D3DCOLOR_ARGB(m_iFadeInOutValue, 255, 255, 255));
		m_pDeviceMgr->GetSprite()->End();

	}
	if(m_pKeyMgr->IsKeyPressing(KEY_D)){
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

	return S_OK;
}

CTitle * CTitle::Create(){
	CTitle* pInstance = new CTitle;
	if(FAILED(pInstance->Initialize())){
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}

unsigned CTitle::Loading(void * pParam){
	CTitle* pTitle = reinterpret_cast<CTitle*>(pParam);

	EnterCriticalSection(&pTitle->m_CriticalSection);

	HRESULT hr = pTitle->m_pTextureMgr->LoadTextureFromPathFile(L"../Data/TextruePath.txt");
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"Texture Load Falied", L"Error", MB_OK);
		return E_FAIL;
	}

	LeaveCriticalSection(&pTitle->m_CriticalSection);

	return 0;
}
