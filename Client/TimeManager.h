#ifndef __TIME_MANAGER_H__
#define __TIME_MANAGER_H__

class CTimeManager{
	DECLARE_SINGLETON(CTimeManager);
private:
	LARGE_INTEGER m_OriginTime;
	LARGE_INTEGER m_OldTime;
	LARGE_INTEGER m_CurTime;
	LARGE_INTEGER m_CPUTick;

	float m_fDeltaTime;         //�� �����Ӵ� �ɸ��� �ð� 60������ ȯ�濡���� 1/60��

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
