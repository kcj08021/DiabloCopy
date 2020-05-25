#ifndef __SINGLE_TEXTURE_H__
#define __SINGLE_TEXTURE_H__

#include "Texture.h"
class CSingleTexture:
	public CTexture{
private:
	TEXTURE_INFO* m_pTextureInfo;

private:
	explicit CSingleTexture();
public:
	virtual ~CSingleTexture();

public:
	virtual const TEXTURE_INFO* GetTextureInfo(const wstring& _wstrStateKey = L"", UINT _iIndex = 0) { return m_pTextureInfo; }

	// CTexture을(를) 통해 상속됨
private:
	virtual void Release() override;
public:
	virtual HRESULT LoadTexture(const wstring& _wstrFilePath, const wstring& _wstrDirectionKey = L"", UINT _iImageCount = 0, const DWORD _TransColor = 0, const D3DXVECTOR3& _vecCenter = {}) override;
	virtual HRESULT SetTextureCenter(const D3DXVECTOR3 & _vec3Center, const wstring & _wstrDirectionKey = L"") override;
	virtual HRESULT SetTextureTrans(const DWORD _TransColor, const wstring & _wstrDirectionKey = L"") override;

	//생성 함수
public:
	static CSingleTexture* Create(const wstring& _wstrFilePath, const DWORD _TransColor, const D3DXVECTOR3& _vecCenter);
};

#endif // !__SINGLE_TEXTURE_H__


