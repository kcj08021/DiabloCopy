#ifndef __MULTI_TEXTURE_H__
#define __MULTI_TEXTURE_H__

#include "Texture.h"
class CMultiTexture:
	public CTexture{
private:
	typedef  map<wstring, vector<TEXTURE_INFO*>> TEXTURE_MAP;

private:
	map<wstring, vector<TEXTURE_INFO*>> m_mapTexture;

private:
	explicit CMultiTexture();
public:
	virtual ~CMultiTexture();

	// CTexture을(를) 통해 상속됨
public:
	virtual const TEXTURE_INFO* GetTextureInfo(const wstring& _wstrDirectionKey = L"", UINT _iIndex = 0) override;

private:
	virtual void Release() override;

public:
	virtual HRESULT LoadTexture(const wstring& _wstrFilePath, const wstring& _wstrDirectionKey = L"", UINT _iImageCount = 0, const DWORD _TransColor = 0, const D3DXVECTOR3& _vecCenter = {}) override;
	virtual HRESULT SetTextureCenter(const D3DXVECTOR3& _vec3Center, const wstring & _wstrDirectionKey = L"") override;
	virtual HRESULT SetTextureTrans(const DWORD _TransColor, const wstring & _wstrDirectionKey = L"") override;

public:
	static CMultiTexture* Create(const wstring& _wstrFilePath, const wstring& _wstrDirectionKey = L"", UINT _iImageCount = 0, const DWORD _TransColor = 0, const D3DXVECTOR3& _vecCenter = {});
};


#endif // !__MULTI_TEXTURE_H__
