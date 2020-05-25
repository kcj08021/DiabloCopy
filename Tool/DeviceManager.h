#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

class CDeviceManager{
	//싱글톤 처리
	DECLARE_SINGLETON(CDeviceManager);

	//멤버 열거형 선언
public:
	enum MODE{ MODE_FULLSCREEN, MODE_WINDOWED};

	//멤버변수
private:
	LPDIRECT3D9			m_pSDK;         //IDrect3D9의 포인터, 장치를 조사 및 검증 하고, IDrect3Device9를 생성하는 역할.
	LPDIRECT3DDEVICE9	m_pDevice;		//IDrect3Device9의 포인터, 장치를 통해서 그래픽 렌더링을 수행하는 역할.

	LPD3DXSPRITE		m_pSprite;		//ID3DXSprite의 포인터, 다이렉트 상에서 2D텍스쳐 렌더링을 수행하는 COM객체.
	LPD3DXFONT			m_pFont;		//iD3DXFont의 포인터
	LPD3DXLINE			m_pLine;		
	//여기서 I는 Interface의 약자
	//이렇게 I로 시작하는 자료형을 COM객체 라고 부른다.(COM: Component Object Model)

	//생성자와 소멸자
private:
	explicit CDeviceManager();
	virtual ~CDeviceManager();

	//Get 함수
public:
	LPDIRECT3DDEVICE9 GetDevice(){ return m_pDevice; }
	LPD3DXSPRITE GetSprite(){ return m_pSprite; }
	LPD3DXFONT GetFont(){ return m_pFont; }
	LPD3DXLINE GetLine(){ return m_pLine; }

private:
	void Release();

public:
	HRESULT InitDevice(MODE _eMode);       //장치 초기화 함수
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);
};

#define DEVICEMGR CDeviceManager::GetInstance()

#endif // !__DEVICE_MANAGER_H__
