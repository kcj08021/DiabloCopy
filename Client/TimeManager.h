#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

class CTimeManager{
	DECLARE_SINGLETON(CTimeManager);
private:
	LARGE_INTEGER m_OriginTime;
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_CPUTick;

	float m_fDeltaTime;         //한 프레임당 걸리는 시간 60프레임 환경에서는 1/60초

private:
	explicit CTimeManager();
	virtual ~CTimeManager();

public:
	float GetDeltaTime(){ return m_fDeltaTime; }

public:
	HRESULT InitTime();
	void UpdateTime();
};

#define TIMEMGR CTimeManager::GetInstance()

#endif // !__TIME_MANAGER_H__
