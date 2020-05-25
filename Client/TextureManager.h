#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__
class CTexture;
class CTextureManager{
	DECLARE_SINGLETON(CTextureManager);
public:
	enum TEXTURE_TYPE{ SINGLE_TEXTURE, MULTI_TEXTURE };

private:
	map<wstring, map<wstring, map<wstring, CTexture*>>> m_mapTexture;

private:
	explicit CTextureManager();
	virtual ~CTextureManager();

public:
	const TEXTURE_INFO* GetTextureInfo(	
		const wstring& _wstrTypeKey,
		const wstring& _wstrObjectKey, 
		const int _iIndex = 0,
		const wstring& _wstrStateKey = L"",
		const wstring& _wstrDirectionKey = L""
		);

	HRESULT SetTextureTrans(
		const DWORD _TransColor,
		const wstring& _wstrTypeKey,
		const wstring& _wstrObjectKey,
		const wstring& _wstrStateKey = L"",
		const wstring& _wstrDirectionKey = L"");

	HRESULT SetTextureCenter(
		const D3DXVECTOR3& _vec3Center,
		const wstring& _wstrTypeKey,
		const wstring& _wstrObjectKey,
		const wstring& _wstrStateKey = L"",
		const wstring& _wstrDirectionKey = L"");

	HRESULT LoadTexture(
		TEXTURE_TYPE _eType,
		const wstring& _wstrFilePath,
		const wstring& _wstrTypeKey,
		const wstring& _wstrObjectKey,
		const int _iIndex = 0,
		const DWORD _TransColor = 0,
		const D3DXVECTOR3& _vecCenter = {},
		const wstring& _wstrStateKey = L"",
		const wstring& _wstrDirectionKey = L""
		);
	HRESULT LoadTextureFromPathFile(TCHAR* wstrPath);
private:
	void Release();
};

#define TEXTUREMGR CTextureManager::GetInstance()
#endif // !__TEXTURE_MANAGER_H__


