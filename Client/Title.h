#ifndef __TITLE_H__
#define __TITLE_H__

#include "Scene.h"
class CDeviceManager;
class CTextureManager;
class CKeyManager;

class CTitle:
	public CScene{
private:
	int m_iImgFrame;
	int m_iImgCount;
	float m_fDeltaTime;
	RECT m_tRect;
	wstring wstrLoadingEnd;
	bool m_bIsLoadingEnd;
	
	byte m_iFadeInValue;
	byte m_iFadeInOutValue;
	float m_fCosine;

	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;
	CTimeManager* m_pTimeMgr;
	CKeyManager* m_pKeyMgr;

	HANDLE				m_LoadingThread;
	CRITICAL_SECTION	m_CriticalSection;

public:
	explicit CTitle();
	virtual ~CTitle();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
public:
	static CTitle* Create();
	static unsigned __stdcall Loading(void* pParam);

};


#endif // !__TITLE_H__

