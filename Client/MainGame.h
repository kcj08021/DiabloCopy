#ifndef __MAINGAME_H__
#define __MAINGAME_H__

class CDeviceManager;
class CTextureManager;
class CSoundManager;
class CSceneManager;
class CKeyManager;
class CTimeManager;
class CFrameManager;
class CScrollManager;
class CObjectManager;

class CMainGame {
private:
	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;
	CSceneManager* m_pSceneMgr;
	CSoundManager* m_pSoundMgr;
	CKeyManager* m_pKeyMgr;
	CTimeManager* m_pTimeMgr;
	CFrameManager* m_pFrameMgr;



private:
	explicit CMainGame();
public:
	virtual ~CMainGame();

	// 기본 구동 함수
private:
	HRESULT Initialize();
	void Release();
public:
	void Update();
	void LateUpdate();
	void Render();

	// 정적 멤버 함수
public:
	static CMainGame* Create();
};

#endif // !__MAINGAME_H__
