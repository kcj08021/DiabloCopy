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
	//nullptr üũ
	if(nullptr == pDevice){
		MessageBox(g_hWnd, L"CSingleTexture::LoadTexture()���� DEVICEMGR->GetDevice()�� nullptr�Դϴ�.", L"ERROR", MB_OK);
		return E_FAIL;
	}
	
	TEXTURE_MAP::iterator find_iter = m_mapTexture.find(_wstrDirectionKey);
	
	if(m_mapTexture.end() != find_iter){
		return E_FAIL;
	}

	for(size_t i = 0; i < _iImageCount; ++i){
		swprintf_s(wstrFullFilePath, _wstrFilePath.c_str(), i);

		//���Ϸ� ���� �̹����� ������ �о��
		hr = D3DXGetImageInfoFromFile(wstrFullFilePath, &tImgInfo);
		//���� üũ
		if(FAILED(hr)){
			MessageBox(g_hWnd, wstrFullFilePath, L"�̹��� ���� �ҷ����� ����", MB_OK);
			return E_FAIL;
		}

		hr = D3DXCreateTextureFromFileEx(
			pDevice,				//��ġ
			wstrFullFilePath,	//�̹��� ���� ���
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

			_TransColor,			//������ ����
			nullptr,				//�ؽ��� ����(������ ����� tImgInfo�̱� ������ �ʿ䰡 ����)
			nullptr,				//PALETTERGB�� ������(���ſ� ARGB���� �̿��Ҷ� ���� ��, ������ ��� ����)
			&pTexture				//LPDIRECT3DTEXTURE9���� ������, ���� �ؽ��ĸ� �ޱ����� ����
		);
		//����üũ
		if(FAILED(hr)){
			MessageBox(g_hWnd, wstrFullFilePath, L"�̹��� �ҷ����� ����", MB_OK);
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

