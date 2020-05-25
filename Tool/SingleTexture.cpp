#include "stdafx.h"
#include "SingleTexture.h"

CSingleTexture::CSingleTexture(){

}

CSingleTexture::~CSingleTexture(){
	Release();
}

void CSingleTexture::Release(){
	m_pTextureInfo->pTexture->Release();        //�ؽ��ĸ� ��� �ִ� ��ü�� COM��ü�̱� ������ Release()�Լ��� ������ �ִ�.
												//�ݵ�� ���� Release()�� �� �ڿ� ����ü�� delete����
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
	//nullptr üũ
	if(nullptr == pDevice){
		MessageBox(g_hWnd, L"CSingleTexture::LoadTexture()���� DEVICEMGR->GetDevice()�� nullptr�Դϴ�.", L"ERROR", MB_OK);
		return E_FAIL;
	}
	
	//���Ϸ� ���� �̹����� ������ �о��
	hr = D3DXGetImageInfoFromFile(_wstrFilePath.c_str(), &tImgInfo);
	//���� üũ
	if(FAILED(hr)){
		MessageBox(g_hWnd, _wstrFilePath.c_str(), L"�̹��� ���� �ҷ����� ����", MB_OK);
		return E_FAIL;
	}
	
	hr = D3DXCreateTextureFromFileEx(
		pDevice,				//��ġ
		_wstrFilePath.c_str(),	//�̹��� ���� ���
		tImgInfo.Width,			//�̹��� ���� ũ��
		tImgInfo.Height,		//�̹��� ���� ũ��
		tImgInfo.MipLevels,		//�̹����� �Ӹ� ����
		0,						//�ؽ��� �����(�Ϲ����� �ؽ��ķ� ����Ҷ� 0,����Ʈ)
		tImgInfo.Format,		//�̹����� �ȼ� ����

		D3DPOOL_MANAGED,		//�޸� �����
								//D3DPOOL_DEFAULT: �⺻��(VRAM�� ���)
								//D3DPOOL_MANAGED: �׷���ī���� VRAM�� ��� �ϸ�, RAM�� �����
								//D3DPOOL_SYSTEMMEM: �ý��� �޸�(RAM)
								//�ӵ��� ������� �ӵ��� �������� �������� ����

		D3DX_DEFAULT,			//�ؽ��� ���͸� ���
		D3DX_DEFAULT,			//�Ӹ� ���͸� ���
								//���͸��� �ؽ��ĸ� Ȯ�������� ���̻��� �� �ȼ��� ��� ä��������� ���� ��

		_TransColor,						//������ ����
		nullptr,				//�ؽ��� ����(������ ����� tImgInfo�̱� ������ �ʿ䰡 ����)
		nullptr,				//PALETTERGB�� ������(���ſ� ARGB���� �̿��Ҷ� ���� ��, ������ ��� ����)
		&pTexture				//LPDIRECT3DTEXTURE9���� ������, ���� �ؽ��ĸ� �ޱ����� ����
	);
	//����üũ
	if(FAILED(hr)){
		MessageBox(g_hWnd, _wstrFilePath.c_str(), L"�̹��� �ҷ����� ����", MB_OK);
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


