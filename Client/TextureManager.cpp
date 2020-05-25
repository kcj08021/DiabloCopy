#include "stdafx.h"
#include "TextureManager.h"
#include "SingleTexture.h"
#include "MultiTexture.h"

IMPLEMENT_SINGLETON(CTextureManager);

CTextureManager::CTextureManager(){

}

CTextureManager::~CTextureManager(){
	Release();
}

const TEXTURE_INFO* CTextureManager::GetTextureInfo(const wstring& _wstrTypeKey, const wstring& _wstrObjectKey, const int _iIndex/* = 0*/, const wstring& _wstrStateKey/* = L""*/, const wstring& _wstrDirectionKey/* = L""*/){
	auto find_TypeIter = m_mapTexture.find(_wstrTypeKey);

	if(m_mapTexture.end() == find_TypeIter)
		return nullptr;
	else{
		auto find_ObjectIter = m_mapTexture[_wstrTypeKey].find(_wstrObjectKey);

		if(m_mapTexture[_wstrTypeKey].end() == find_ObjectIter)
			return nullptr;
		else{
			auto find_StateIter = m_mapTexture[_wstrTypeKey][_wstrObjectKey].find(_wstrStateKey);

			if(m_mapTexture[_wstrTypeKey][_wstrObjectKey].end() == find_StateIter)
				return nullptr;
		}
	}

	return m_mapTexture[_wstrTypeKey][_wstrObjectKey][_wstrStateKey]->GetTextureInfo(_wstrDirectionKey, _iIndex);
}

HRESULT CTextureManager::SetTextureTrans(const DWORD _TransColor, const wstring& _wstrTypeKey, const wstring & _wstrObjectKey, const wstring & _wstrStateKey, const wstring & _wstrDirectionKey){
	auto find_TypeIter = m_mapTexture.find(_wstrTypeKey);

	if(m_mapTexture.end() == find_TypeIter)
		return E_FAIL;
	else{
		auto find_ObjectIter = m_mapTexture[_wstrTypeKey].find(_wstrObjectKey);

		if(m_mapTexture[_wstrTypeKey].end() == find_ObjectIter)
			return E_FAIL;
		else{
			auto find_StateIter = m_mapTexture[_wstrTypeKey][_wstrObjectKey].find(_wstrStateKey);

			if(m_mapTexture[_wstrTypeKey][_wstrObjectKey].end() == find_StateIter)
				return E_FAIL;
		}
	}

	return m_mapTexture[_wstrTypeKey][_wstrObjectKey][_wstrStateKey]->SetTextureTrans(_TransColor, _wstrDirectionKey);
}

HRESULT CTextureManager::SetTextureCenter(const D3DXVECTOR3& _vec3Center, const wstring& _wstrTypeKey, const wstring & _wstrObjectKey, const wstring & _wstrStateKey, const wstring & _wstrDirectionKey){
	auto find_TypeIter = m_mapTexture.find(_wstrTypeKey);

	if(m_mapTexture.end() == find_TypeIter)
		return E_FAIL;
	else{
		auto find_ObjectIter = m_mapTexture[_wstrTypeKey].find(_wstrObjectKey);

		if(m_mapTexture[_wstrTypeKey].end() == find_ObjectIter)
			return E_FAIL;
		else{
			auto find_StateIter = m_mapTexture[_wstrTypeKey][_wstrObjectKey].find(_wstrStateKey);

			if(m_mapTexture[_wstrTypeKey][_wstrObjectKey].end() == find_StateIter)
				return E_FAIL;
		}
	}

	return m_mapTexture[_wstrTypeKey][_wstrObjectKey][_wstrStateKey]->SetTextureCenter(_vec3Center, _wstrDirectionKey);
}

HRESULT CTextureManager::LoadTexture(TEXTURE_TYPE _eType, const wstring& _wstrFilePath, const wstring& _wstrTypeKey, const wstring& _wstrObjectKey, const int _iCount/* = 0*/, const DWORD _TransColor/* = 0*/, const D3DXVECTOR3& _vecCenter/* = {}*/, const wstring& _wstrStateKey/* = L""*/, const wstring& _wstrDirectionKey/* = L""*/){
	auto find_TypeIter = m_mapTexture.find(_wstrTypeKey);

	CTexture* pTexture = nullptr;

	if(m_mapTexture.end() == find_TypeIter){
		switch(_eType){
		case CTextureManager::SINGLE_TEXTURE:
			pTexture = CSingleTexture::Create(_wstrFilePath, _TransColor, _vecCenter);
			break;
		case CTextureManager::MULTI_TEXTURE:
			pTexture = CMultiTexture::Create(_wstrFilePath, _wstrDirectionKey, _iCount, _TransColor, _vecCenter);
			break;
		default:
			break;
		}

		if(nullptr == pTexture)
			return E_FAIL;

		m_mapTexture[_wstrTypeKey][_wstrObjectKey][_wstrStateKey] = pTexture;
	} else{
		auto find_ObjectIter = (m_mapTexture[_wstrTypeKey]).find(_wstrObjectKey);

		if(m_mapTexture[_wstrTypeKey].end() == find_ObjectIter){
			switch(_eType){
			case CTextureManager::SINGLE_TEXTURE:
				pTexture = CSingleTexture::Create(_wstrFilePath, _TransColor, _vecCenter);
				break;
			case CTextureManager::MULTI_TEXTURE:
				pTexture = CMultiTexture::Create(_wstrFilePath, _wstrDirectionKey, _iCount, _TransColor, _vecCenter);
				break;
			default:
				break;
			}

			if(nullptr == pTexture)
				return E_FAIL;

			m_mapTexture[_wstrTypeKey][_wstrObjectKey][_wstrStateKey] = pTexture;
		} else{
			auto find_StateIter = m_mapTexture[_wstrTypeKey][_wstrObjectKey].find(_wstrStateKey);

			if(m_mapTexture[_wstrTypeKey][_wstrObjectKey].end() == find_StateIter){
				if(CTextureManager::SINGLE_TEXTURE == _eType){
					pTexture = CSingleTexture::Create(_wstrFilePath, _TransColor, _vecCenter);
				} else{
					pTexture = CMultiTexture::Create(_wstrFilePath, _wstrDirectionKey, _iCount, _TransColor, _vecCenter);
				}
				if(nullptr == pTexture)
					return E_FAIL;

				m_mapTexture[_wstrTypeKey][_wstrObjectKey][_wstrStateKey] = pTexture;
			} else{
				HRESULT hr = m_mapTexture[_wstrTypeKey][_wstrObjectKey][_wstrStateKey]->LoadTexture(_wstrFilePath, _wstrDirectionKey, _iCount, _TransColor, _vecCenter);
			
				if(FAILED(hr))
					return E_FAIL;
			}
		}
	}

	return S_OK;
}

HRESULT CTextureManager::LoadTextureFromPathFile(TCHAR * wstrPath){
	wifstream fin;

	fin.open(wstrPath);

	if(fin.fail())
		return E_FAIL;

	TCHAR wstrTypeKey[MAX_STR] = L"";
	TCHAR wstrObjectKey[MAX_STR] = L"";
	TCHAR iImgCount[MAX_STR] = L"";
	TCHAR wstrStateKey[MAX_STR] = L"";
	TCHAR wstrDirectionKey[MAX_STR] = L"";
	TCHAR iColorA[MAX_STR] = L"";
	TCHAR iColorR[MAX_STR] = L"";
	TCHAR iColorG[MAX_STR] = L"";
	TCHAR iColorB[MAX_STR] = L"";
	TCHAR iCenterX[MAX_STR] = L"";
	TCHAR iCenterY[MAX_STR] = L"";
	TCHAR wstrImgPath[MAX_STR] = L"";

	fin.getline(wstrTypeKey, MAX_STR);

	while(!fin.eof()){
		fin.getline(wstrTypeKey, MAX_STR, '|');
		fin.getline(wstrObjectKey, MAX_STR, '|');
		fin.getline(iImgCount, MAX_STR, '|');
		fin.getline(wstrStateKey, MAX_STR, '|');
		fin.getline(wstrDirectionKey, MAX_STR, '|');
		fin.getline(iColorA, MAX_STR, '|');
		fin.getline(iColorR, MAX_STR, '|');
		fin.getline(iColorG, MAX_STR, '|');
		fin.getline(iColorB, MAX_STR, '|');
		fin.getline(iCenterX, MAX_STR, '|');
		fin.getline(iCenterY, MAX_STR, '|');
		fin.getline(wstrImgPath, MAX_STR);


		if(_ttoi(iImgCount) != 1){
			HRESULT hr = LoadTexture(MULTI_TEXTURE,
				wstrImgPath,
				wstrTypeKey,
				wstrObjectKey,
				_ttoi(iImgCount),
				D3DCOLOR_ARGB(_ttoi(iColorA), _ttoi(iColorR), _ttoi(iColorG), _ttoi(iColorB)),
				D3DXVECTOR3(static_cast<float>(_ttof(iCenterX)), static_cast<float>(_ttof(iCenterY)), 0.f),
				wstrStateKey,
				wstrDirectionKey);
		} else{
			HRESULT hr = LoadTexture(SINGLE_TEXTURE,
				wstrImgPath,
				wstrTypeKey,
				wstrObjectKey,
				_ttoi(iImgCount),
				D3DCOLOR_ARGB(_ttoi(iColorA), _ttoi(iColorR), _ttoi(iColorG), _ttoi(iColorB)),
				D3DXVECTOR3(static_cast<float>(_ttof(iCenterX)), static_cast<float>(_ttof(iCenterY)), 0.f),
				wstrStateKey,
				wstrDirectionKey);
		}
	}

	fin.close();

	return S_OK;
}


void CTextureManager::Release(){
	for(auto& TypePair : m_mapTexture){
		for(auto& ObjectPair : TypePair.second){
			for(auto& StatePair : ObjectPair.second){
				SafeDelete(StatePair.second);
			}
			ObjectPair.second.clear();
		}
		TypePair.second.clear();
	}

	m_mapTexture.clear();
}
