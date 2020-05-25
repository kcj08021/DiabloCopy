#include "stdafx.h"
#include "DeviceManager.h"

IMPLEMENT_SINGLETON(CDeviceManager);

CDeviceManager::CDeviceManager(){

}

CDeviceManager::~CDeviceManager(){
	Release();
}

void CDeviceManager::Release(){
	//COM객체 해제, COM객체는 다 Release()함수를 가지고 있다.

	//COM객체 해제, Device객체를 먼저 해제하고, SDK객체를 해제해야 한다.
	//COM객체는 레퍼런스 카운트 기법을 이용하여 얕은 복사를 사용한다.
	//따라서 SDK객체로 만들어진 Device를 먼저 해제하고 SDK를 해제해야 한다.
	//COM객체의 Release()함수는 ULONG타입의 값을 반환한다. 이때 반환하는 것이 레퍼런스 카운터의 값.
	if(m_pLine->Release())
		MessageBox(0, L"m_pLine Release Failed", L"ERROR", MB_OK);
	if(m_pFont->Release())
		MessageBox(0, L"m_pFont Release Failed", L"ERROR", MB_OK);
	if(m_pSprite->Release())
		MessageBox(0, L"m_pSDK Release Failed", L"ERROR", MB_OK);
	if(m_pDevice->Release())
		MessageBox(0, L"m_pSDK Release Failed", L"ERROR", MB_OK);
	if(m_pSDK->Release())
		MessageBox(0, L"m_pSDK Release Failed", L"ERROR", MB_OK);
}

HRESULT CDeviceManager::InitDevice(MODE _eMode){
	HRESULT hr = 0;
	//장치 초기화

	//1. IDrect3D9 객체 생성
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);          //D3D_SDK_VERSION는 매크로 상수로 다이렉트X 버전에 따라 알아서 상수 값이 잡힌다.

	if(m_pSDK == nullptr){
		return E_FAIL;
	}

	//2. 장치 조사
	//2-1. HAL을 통해 그래픽 장치 조사(HAL; Hardware Abstraction Layer, 하드웨어 추상화 계층)

	D3DCAPS9 d3dcaps;                                       //그래픽 카드에 대한 정보가 들어가는 구조체.
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	//GetDeviceCaps()함수로 그래픽카드 정보를 조사하여 마지막 인자의 D3DCAPS9 구조체 변수에 결과를 넣음
	hr = m_pSDK->GetDeviceCaps(
		D3DADAPTER_DEFAULT,                                 //D3DADAPTER_DEFAULT는 장착된 그래픽 카드의 기본값                     
		D3DDEVTYPE_HAL,                                     //D3DDEVTYPE_HAL는 HAL을 이용하여 장치를 조사한다는 것
		&d3dcaps
	);

	if(FAILED(hr)){

		return E_FAIL;
	}
	//해당 함수는 결과로 HRESULT를 반환한다.(FAIL 체크를 하면 좋다.)

	//2-2. 해당 장치가 버텍스 프로세싱을 지원하는지 조사.    *버텍스 프로세싱: 정점 변환 + 조명 처리, 게임을 만드는데 필요한 기술들

	DWORD vp = 0;

	if(d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//3. IDrect3Device9 객체 생성

	D3DPRESENT_PARAMETERS d3dpp;                                //현재 렌더링 환경을 담는 구조체, 개발자 자신이 값을 채워넣어야함.
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	//d3dpp 값 채우기
	d3dpp.BackBufferWidth = WINCX;                              //후면 버퍼 가로크기
	d3dpp.BackBufferHeight = WINCY;                             //후면 버퍼 세로크기
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;		            //후면 버퍼 픽셀 포맷(D3DFMT_A8R8G8B8: ARGB 각 8비트라는 의미의 상수)
	d3dpp.BackBufferCount = 1;                                  //후면 버퍼의 갯수(1을 준다.)

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;                //멀티 샘플링 종류(NONE은 쓰지 않는다.)
	d3dpp.MultiSampleQuality = 0;                               //멀티 샘플링 품질(NONE일때 0)

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   //화면 깜박이는 것을 어떤 방식으로 해결할 것인가 에 대한 상수값
																//D3DSWAPEFFECT_DISCARD: 스왑 체인 방식(한 프레임마다 버퍼 두개를 번갈아가며 출력, 이러한 방식을 페이지 플립핑 방식이라 함)
																//D3DSWAPEFFECT_FLIP: 
																//D3DSWAPEFFECT_COPY: API에서 사용했던 더블 버퍼링 방법과 같은 복사 방법

	d3dpp.hDeviceWindow = g_hWnd;                               //장치를 사용할 창
	d3dpp.Windowed = _eMode;									//창모드 여부(MODE_WINDOWED는 enum으로 1이기 때문에 TRUE)

	d3dpp.EnableAutoDepthStencil = TRUE;                        //깊이 버퍼와 스텐실 버퍼를 자동으로 관리하는지에 대한 BOOL값
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;                //깊이 버퍼와 스텐실 버퍼에 대한 포맷값(D3DFMT_D24S8: 깊이 24비트, 스텐실 8비트)
																//깊이 버퍼:    깊이값을 표현하기 위한 버퍼(깊이는 원근을 표현하기 위한 방법, 깊이가 있어야 3D)
																//스텐실 버퍼:  후면 버퍼에 렌더링 되는 것이 막는 기법(후면 버퍼에 렌더가 될것과 안될것을 구분 하는 버퍼)

	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //전체화면일때 사용할 주사율, 창모드일땐 0을 준다.(창모드일때는 OS가 알아서 주사율을 설정함)
																//D3DPRESENT_RATE_DEFAULT는 현재 사용중인 모니터의 주사율
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //간격 처리방법(D3DPRESENT_INTERVAL_IMMEDIATE는 간격이 발생해도 즉시 시연)
	//

	hr = m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice);
	if(FAILED(hr)){
		return E_FAIL;
	}

	hr = D3DXCreateSprite(m_pDevice, &m_pSprite);
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"DEVICEMGR->InitDevice() Sprite Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	D3DXFONT_DESC tFontInfo;
	INIT_STRUCT(tFontInfo);

	tFontInfo.Width = 10;
	tFontInfo.Height = 20;
	tFontInfo.Weight = FW_HEAVY;
	tFontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(tFontInfo.FaceName, L"맑은 고딕");

	hr = D3DXCreateFontIndirect(m_pDevice, &tFontInfo, &m_pFont);
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"DEVICEMGR->InitDevice() Font Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	hr = D3DXCreateLine(m_pDevice, &m_pLine);
	if(FAILED(hr)){
		::MessageBox(g_hWnd, L"DEVICEMGR->InitDevice() Line Create Failed", L"Error", MB_OK);
		return E_FAIL;
	}
	m_pLine->SetWidth(2.f);
	m_pLine->SetAntialias(TRUE);


	return S_OK;
}

void CDeviceManager::Render_Begin(){
	//장치에 렌더링 하는 과정
	//1. 후면버퍼 비우기(Clear)
	m_pDevice->Clear(
		0,														//사각형 단위로 비울때 사각형의 갯수(0일때 후면버퍼 전체)
		nullptr,												//사각형의 배열(전체일땐 nullptr)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//어떤 버퍼를 비우는지에 대한 플래그값
																//각각 픽셀(TARGER),깊이(ZBUFFER), 스텐실(STENSIL)버퍼
		D3DCOLOR_ARGB(255, 0, 0, 0),							//픽셀 버퍼를 채울 색상 지정
		1.f,													//깊이 버퍼를 채움(Z버퍼는 1에서 0의 값을 갖는데, 1이 가장 먼 것, 0이 제일 가까운것)
		0														//스텐실 버퍼 채움(0으로 비움)
	);
	//파란색인 이유는 조명계산을 실수했을때 흰색과 검정색으로 표현되기 때문에, 보통 원색을 사용하여 채운다.
	//현재 알파(A) 부분에 0을 주더라도 실제로 투명하게 나오지는 않는다.(투명하게 처리할 대상이 없기 때문에)
	//Z버퍼를 갱신할땐 Z값이 작은것만 갱신된다.(대충 알고 넘어가고 3D때 배움)
	
	//2. 후면버퍼에 그리기 시작(Begin)
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);		//D3DXSPRITE_ALPHABLEND: 알파값이 있다면 투명도 적용
}

void CDeviceManager::Render_End(HWND hWnd/* = nullptr*/){
	//3. 후면버퍼에 그리기 끝내기(End)
	m_pSprite->End();
	m_pDevice->EndScene();

	//4. 후면버퍼를 전면버퍼로 교체하여 출력(Present)
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);		//세번째 인자는 윈도우 핸들이 들어감, nullptr일때는 장치 초기화 할떄 넣어준 윈도우 핸들을 사용한다.
}
