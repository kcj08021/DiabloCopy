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

	//�ֱ������� CPU�� �������� ����.
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
		DEVICEMGR->GetSprite(),				//Sprite COM��ü�� ������(LPD3DXSPRITE)
		m_szFps,							//����� ���ڿ�
		lstrlen(m_szFps),					//���ڿ� ũ��
		nullptr,							//���ڿ��� ����� �簢��(���� ��ġ�� ��ķ� ��� ������ ����)
		0,									//�簢���� ����� �ɼ�
		D3DCOLOR_ARGB(255, 255, 0, 0)	//����� ����
	);
	DEVICEMGR->GetSprite()->End();

}

