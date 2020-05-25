#include "stdafx.h"
#include "DeviceManager.h"

IMPLEMENT_SINGLETON(CDeviceManager);

CDeviceManager::CDeviceManager(){

}

CDeviceManager::~CDeviceManager(){
	Release();
}

void CDeviceManager::Release(){
	//COM��ü ����, COM��ü�� �� Release()�Լ��� ������ �ִ�.

	//COM��ü ����, Device��ü�� ���� �����ϰ�, SDK��ü�� �����ؾ� �Ѵ�.
	//COM��ü�� ���۷��� ī��Ʈ ����� �̿��Ͽ� ���� ���縦 ����Ѵ�.
	//���� SDK��ü�� ������� Device�� ���� �����ϰ� SDK�� �����ؾ� �Ѵ�.
	//COM��ü�� Release()�Լ��� ULONGŸ���� ���� ��ȯ�Ѵ�. �̶� ��ȯ�ϴ� ���� ���۷��� ī������ ��.
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
	//��ġ �ʱ�ȭ

	//1. IDrect3D9 ��ü ����
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);          //D3D_SDK_VERSION�� ��ũ�� ����� ���̷�ƮX ������ ���� �˾Ƽ� ��� ���� ������.

	if(m_pSDK == nullptr){
		return E_FAIL;
	}

	//2. ��ġ ����
	//2-1. HAL�� ���� �׷��� ��ġ ����(HAL; Hardware Abstraction Layer, �ϵ���� �߻�ȭ ����)

	D3DCAPS9 d3dcaps;                                       //�׷��� ī�忡 ���� ������ ���� ����ü.
	ZeroMemory(&d3dcaps, sizeof(D3DCAPS9));

	//GetDeviceCaps()�Լ��� �׷���ī�� ������ �����Ͽ� ������ ������ D3DCAPS9 ����ü ������ ����� ����
	hr = m_pSDK->GetDeviceCaps(
		D3DADAPTER_DEFAULT,                                 //D3DADAPTER_DEFAULT�� ������ �׷��� ī���� �⺻��                     
		D3DDEVTYPE_HAL,                                     //D3DDEVTYPE_HAL�� HAL�� �̿��Ͽ� ��ġ�� �����Ѵٴ� ��
		&d3dcaps
	);

	if(FAILED(hr)){

		return E_FAIL;
	}
	//�ش� �Լ��� ����� HRESULT�� ��ȯ�Ѵ�.(FAIL üũ�� �ϸ� ����.)

	//2-2. �ش� ��ġ�� ���ؽ� ���μ����� �����ϴ��� ����.    *���ؽ� ���μ���: ���� ��ȯ + ���� ó��, ������ ����µ� �ʿ��� �����

	DWORD vp = 0;

	if(d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//3. IDrect3Device9 ��ü ����

	D3DPRESENT_PARAMETERS d3dpp;                                //���� ������ ȯ���� ��� ����ü, ������ �ڽ��� ���� ä���־����.
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));

	//d3dpp �� ä���
	d3dpp.BackBufferWidth = WINCX;                              //�ĸ� ���� ����ũ��
	d3dpp.BackBufferHeight = WINCY;                             //�ĸ� ���� ����ũ��
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;		            //�ĸ� ���� �ȼ� ����(D3DFMT_A8R8G8B8: ARGB �� 8��Ʈ��� �ǹ��� ���)
	d3dpp.BackBufferCount = 1;                                  //�ĸ� ������ ����(1�� �ش�.)

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;                //��Ƽ ���ø� ����(NONE�� ���� �ʴ´�.)
	d3dpp.MultiSampleQuality = 0;                               //��Ƽ ���ø� ǰ��(NONE�϶� 0)

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;                   //ȭ�� �����̴� ���� � ������� �ذ��� ���ΰ� �� ���� �����
																//D3DSWAPEFFECT_DISCARD: ���� ü�� ���(�� �����Ӹ��� ���� �ΰ��� �����ư��� ���, �̷��� ����� ������ �ø��� ����̶� ��)
																//D3DSWAPEFFECT_FLIP: 
																//D3DSWAPEFFECT_COPY: API���� ����ߴ� ���� ���۸� ����� ���� ���� ���

	d3dpp.hDeviceWindow = g_hWnd;                               //��ġ�� ����� â
	d3dpp.Windowed = _eMode;									//â��� ����(MODE_WINDOWED�� enum���� 1�̱� ������ TRUE)

	d3dpp.EnableAutoDepthStencil = TRUE;                        //���� ���ۿ� ���ٽ� ���۸� �ڵ����� �����ϴ����� ���� BOOL��
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;                //���� ���ۿ� ���ٽ� ���ۿ� ���� ���˰�(D3DFMT_D24S8: ���� 24��Ʈ, ���ٽ� 8��Ʈ)
																//���� ����:    ���̰��� ǥ���ϱ� ���� ����(���̴� ������ ǥ���ϱ� ���� ���, ���̰� �־�� 3D)
																//���ٽ� ����:  �ĸ� ���ۿ� ������ �Ǵ� ���� ���� ���(�ĸ� ���ۿ� ������ �ɰͰ� �ȵɰ��� ���� �ϴ� ����)

	d3dpp.Flags = 0;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //��üȭ���϶� ����� �ֻ���, â����϶� 0�� �ش�.(â����϶��� OS�� �˾Ƽ� �ֻ����� ������)
																//D3DPRESENT_RATE_DEFAULT�� ���� ������� ������� �ֻ���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; //���� ó�����(D3DPRESENT_INTERVAL_IMMEDIATE�� ������ �߻��ص� ��� �ÿ�)
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
	lstrcpy(tFontInfo.FaceName, L"���� ���");

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
	//��ġ�� ������ �ϴ� ����
	//1. �ĸ���� ����(Clear)
	m_pDevice->Clear(
		0,														//�簢�� ������ ��ﶧ �簢���� ����(0�϶� �ĸ���� ��ü)
		nullptr,												//�簢���� �迭(��ü�϶� nullptr)
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//� ���۸� �������� ���� �÷��װ�
																//���� �ȼ�(TARGER),����(ZBUFFER), ���ٽ�(STENSIL)����
		D3DCOLOR_ARGB(255, 0, 0, 0),							//�ȼ� ���۸� ä�� ���� ����
		1.f,													//���� ���۸� ä��(Z���۴� 1���� 0�� ���� ���µ�, 1�� ���� �� ��, 0�� ���� ������)
		0														//���ٽ� ���� ä��(0���� ���)
	);
	//�Ķ����� ������ �������� �Ǽ������� ����� ���������� ǥ���Ǳ� ������, ���� ������ ����Ͽ� ä���.
	//���� ����(A) �κп� 0�� �ִ��� ������ �����ϰ� �������� �ʴ´�.(�����ϰ� ó���� ����� ���� ������)
	//Z���۸� �����Ҷ� Z���� �����͸� ���ŵȴ�.(���� �˰� �Ѿ�� 3D�� ���)
	
	//2. �ĸ���ۿ� �׸��� ����(Begin)
	m_pDevice->BeginScene();
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);		//D3DXSPRITE_ALPHABLEND: ���İ��� �ִٸ� ���� ����
}

void CDeviceManager::Render_End(HWND hWnd/* = nullptr*/){
	//3. �ĸ���ۿ� �׸��� ������(End)
	m_pSprite->End();
	m_pDevice->EndScene();

	//4. �ĸ���۸� ������۷� ��ü�Ͽ� ���(Present)
	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);		//����° ���ڴ� ������ �ڵ��� ��, nullptr�϶��� ��ġ �ʱ�ȭ �ҋ� �־��� ������ �ڵ��� ����Ѵ�.
}
