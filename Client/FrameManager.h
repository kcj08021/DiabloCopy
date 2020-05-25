#ifndef __FRAME_MANAGER_H__
#define __FRAME_MANAGER_H__

class CFrameManager{
	DECLARE_SINGLETON(CFrameManager);

private:
	LARGE_INTEGER m_OriginTime;
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_CPUTick;

	float m_fTimeCount;
	float m_fTimeCount2;

	//프레임 수치를 표기하기 위한 멤버 변수
	TCHAR m_szFps[MIN_STR];
	int m_iFps;

private:
	explicit CFrameManager();
	virtual ~CFrameManager();

public:
	HRESULT InitTime();
	bool LockFrame(float _fFps);
	void RenderFps();
};

#define FRAMEMGR CFrameManager::GetInstance()

#endif // !__FRAME_MANAGER_H__
