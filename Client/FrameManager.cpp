#include "stdafx.h"
#include "FrameManager.h"

IMPLEMENT_SINGLETON(CFrameManager);

CFrameManager::CFrameManager() : m_fTimeCount(0.f), m_fTimeCount2(0.f), m_szFps(L""), m_iFps(0) {
	INIT_STRUCT(m_OriginTime);
	INIT_STRUCT(m_OldTime);
	INIT_STRUCT(m_CurTime);
	INIT_STRUCT(m_CPUTick);
}

CFrameManager::~CFrameManager() {

}

HRESULT CFrameManager::InitTime() {
	QueryPerformanceCounter(&m_OriginTime);
	QueryPerformanceCounter(&m_OldTime);
	QueryPerformanceCounter(&m_CurTime);

	QueryPerformanceFrequency(&m_CPUTick);

	return S_OK;
}

bool CFrameManager::LockFrame(float _fFps) {
	QueryPerformanceCounter(&m_CurTime);

	//주기적으로 CPU의 진동수를 갱신.
	if (m_CurTime.QuadPart - m_OriginTime.QuadPart > m_CPUTick.QuadPart) {
		QueryPerformanceFrequency(&m_CPUTick);
		m_OriginTime = m_CurTime;
	}

	m_fTimeCount += static_cast<float>(m_CurTime.QuadPart - m_OldTime.QuadPart) / m_CPUTick.QuadPart;

	m_OldTime = m_CurTime;

	if (1.f / _fFps <= m_fTimeCount) {
		++m_iFps;
		m_fTimeCount = 0.f;
		return true;
	}
	return false;
}

void CFrameManager::RenderFps() {
	m_fTimeCount2 += TIMEMGR->GetDeltaTime();

	if (1.f <= m_fTimeCount2) {
		swprintf_s(m_szFps, L"FPS: %d", m_iFps);
		m_iFps = 0;
		m_fTimeCount2 = 0.f;
	}

	D3DXMATRIX matTrans;
	D3DXMatrixTranslation(&matTrans, WINCX - 100.f, 10.f, 0.f);

	DEVICEMGR->GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	DEVICEMGR->GetSprite()->SetTransform(&matTrans);
	DEVICEMGR->GetFont()->DrawText(
		DEVICEMGR->GetSprite(),				//Sprite COM객체의 포인터(LPD3DXSPRITE)
		m_szFps,							//출력할 문자열
		lstrlen(m_szFps),					//문자열 크기
		nullptr,							//문자열을 출력할 사각형(현재 위치를 행렬로 줬기 때문에 없음)
		0,									//사각형에 출력할 옵션
		D3DCOLOR_ARGB(255, 255, 0, 0)	//출력할 색상
	);
	DEVICEMGR->GetSprite()->End();

}

