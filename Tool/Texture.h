#ifndef __TEXTURE_H__
#define __TEXTURE_H__

class CTexture{
protected:
	explicit CTexture();
public:
	virtual ~CTexture();

public:
	virtual const TEXTURE_INFO* GetTextureInfo(const wstring& _wstrDirectionKey = L"", UINT _iIndex = 0) = 0;

private:
	virtual void Release() = 0;
public:
	virtual HRESULT LoadTexture(const wstring& _wstrFilePath, const wstring& _wstrDirectionKey = L"", UINT _iImageCount = 0, const DWORD _TransColor = 0, const D3DXVECTOR3& _vecCenter = {}) = 0;
	virtual HRESULT SetTextureCenter(const D3DXVECTOR3& _vec3Center, const wstring& _wstrDirectionKey = L"") = 0;
	virtual HRESULT SetTextureTrans(const DWORD _TransColor, const wstring & _wstrDirectionKey = L"") = 0;
};
#endif // !__TEXTURE_H__
