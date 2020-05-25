#include "stdafx.h"
#include "TimeManager.h"

IMPLEMENT_SINGLETON(CTimeManager);

CTimeManager::CTimeManager() : m_fDeltaTime(0.f){
	INIT_STRUCT(m_OriginTime);
	INIT_STRUCT(m_OldTime);
	INIT_STRUCT(m_CurTime);
	INIT_STRUCT(m_CPUTick);
}

CTimeManager::~CTimeManager(){
}

HRESULT CTimeManager::InitTime(){
	//하드웨어(메인보드)가 갖고있는 고해상도 타이머의 값을 얻어오는 함수
	//여기서 말하는 타이머는 일정한 주기마다 누적한 값
	//일정한 주기는 CPU의 클럭에 비롯된다.

	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	//CPU의 초당 클럭 수를 얻어오는 함수
	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

void CTimeManager::UpdateTime(){
	QueryPerformanceCounter(&m_CurTime);

	//주기적으로 CPU의 진동수를 갱신.
	if(m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart){
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	m_fDeltaTime = static_cast<float>(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;

	m_OldTime = m_CurTime;
}