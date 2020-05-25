#include "stdafx.h"
#include "MainGame.h"
#include "Terrain.h"

CMainGame::CMainGame(): 
	m_pDeviceMgr(DEVICEMGR), m_pTextureMgr(TEXTUREMGR), m_pSceneMgr(SCENEMGR), m_pSoundMgr(SNDMGR),
	m_pKeyMgr(KEYMGR), m_pTimeMgr(TIMEMGR), m_pFrameMgr(FRAMEMGR){
}


CMainGame::~CMainGame() {
	Release();
}

HRESULT CMainGame::Initialize(){
	HRESULT hr = 0;

	::srand(static_cast<unsigned int>(time(nullptr)));

	hr = AddFontResource(L"../Data/Exocet.TTF");

	if(m_pDeviceMgr){
		hr = m_pDeviceMgr->InitDevice(CDeviceManager::MODE_WINDOWED);
		if(FAILED(hr)){
			::MessageBox(g_hWnd, L"DeviceMgr Init Falied", L"Error", MB_OK);
			return E_FAIL;
		}
	}

	m_pSoundMgr->Initialize();

	//if(m_pTextureMgr){
	//	hr = m_pTextureMgr->LoadTextureFromPathFile(L"../Data/TextruePath.txt");
	//	if(FAILED(hr)){
	//		::MessageBox(g_hWnd, L"Texture Load Falied", L"Error", MB_OK);
	//		return E_FAIL;
	//	}
	//}

	::ShowCursor(false);
	m_pTextureMgr->LoadTexture(CTextureManager::SINGLE_TEXTURE, L"../Texture/UI/Cursor/Cursor.png", L"UI", L"Cursor", 0, D3DCOLOR_ARGB(255,73,101,101));

	m_pSceneMgr->SetScene(CSceneManager::SCENE_TITLE);

	return S_OK;
}

void CMainGame::Release() {
	SCENEMGR->DestroyInstance();
	OBJMGR->DestroyInstance();
	KEYMGR->DestroyInstance();
	SNDMGR->DestroyInstance();
	SCROLLMGR->DestroyInstance();
	ASTARMGR->DestroyInstance();
	TIMEMGR->DestroyInstance();
	FRAMEMGR->DestroyInstance();
	TEXTUREMGR->DestroyInstance();
	DEVICEMGR->DestroyInstance();

	RemoveFontResource(L"../Data/Exocet.TTF");
}

void CMainGame::Update() {
	m_pKeyMgr->Update();

	//float fSpeed = 100.f * m_pTimeMgr->GetDeltaTime();

	//if(m_pKeyMgr->GetMousePos().x < 0.f)
	//	m_pScrollMgr->MoveScroll(-fSpeed, 0.f);
	//else if(m_pKeyMgr->GetMousePos().x > static_cast<float>(WINCX))
	//	m_pScrollMgr->MoveScroll(fSpeed, 0.f);

	//if(m_pKeyMgr->GetMousePos().y < 0.f)
	//	m_pScrollMgr->MoveScroll(0.f, -fSpeed);
	//else if(m_pKeyMgr->GetMousePos().y > static_cast<float>(WINCY))
	//	m_pScrollMgr->MoveScroll(0.f, fSpeed);

	m_pSoundMgr->UpdateSound();

	m_pSceneMgr->Update();
}

void CMainGame::LateUpdate() {
	m_pSceneMgr->LateUpdate();
}

void CMainGame::Render() {
	m_pDeviceMgr->Render_Begin();
	
	m_pSceneMgr->Render();

	const TEXTURE_INFO* pTexture = m_pTextureMgr->GetTextureInfo(L"UI", L"Cursor");

	if(pTexture != nullptr){
		D3DXMATRIX matTrans, matScale, matWorld;

		D3DXVECTOR3 vMousePos = m_pKeyMgr->GetMousePos();

		D3DXMatrixScaling(&matScale, 2.f, 2.f, 0.f);
		D3DXMatrixTranslation(&matTrans, vMousePos.x, vMousePos.y, 0.f);
		matWorld = matScale * matTrans;
		m_pDeviceMgr->GetSprite()->SetTransform(&matWorld);
		m_pDeviceMgr->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		m_pDeviceMgr->GetSprite()->Draw(pTexture->pTexture, nullptr, &D3DXVECTOR3{ 0.f,0.f,0.f }, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
		m_pDeviceMgr->GetSprite()->End();
	}

	m_pFrameMgr->RenderFps();

	m_pDeviceMgr->Render_End();
}

CMainGame * CMainGame::Create() {
	CMainGame* pInstance = new CMainGame;

	if (FAILED(pInstance->Initialize())) {
		SafeDelete(pInstance);
		return nullptr;
	}

	return pInstance;
}
