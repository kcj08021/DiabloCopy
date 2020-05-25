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
	//�ϵ����(���κ���)�� �����ִ� ���ػ� Ÿ�̸��� ���� ������ �Լ�
	//���⼭ ���ϴ� Ÿ�̸Ӵ� ������ �ֱ⸶�� ������ ��
	//������ �ֱ�� CPU�� Ŭ���� ��Եȴ�.

	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	//CPU�� �ʴ� Ŭ�� ���� ������ �Լ�
	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

void CTimeManager::UpdateTime(){
	QueryPerformanceCounter(&m_CurTime);

	//�ֱ������� CPU�� �������� ����.
	if(m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart){
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	m_fDeltaTime = static_cast<float>(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;

	m_OldTime = m_CurTime;
}