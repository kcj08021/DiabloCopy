#ifndef __DEVICE_MANAGER_H__
#define __DEVICE_MANAGER_H__

class CDeviceManager{
	//�̱��� ó��
	DECLARE_SINGLETON(CDeviceManager);

	//��� ������ ����
public:
	enum MODE{ MODE_FULLSCREEN, MODE_WINDOWED};

	//�������
private:
	LPDIRECT3D9			m_pSDK;         //IDrect3D9�� ������, ��ġ�� ���� �� ���� �ϰ�, IDrect3Device9�� �����ϴ� ����.
	LPDIRECT3DDEVICE9	m_pDevice;		//IDrect3Device9�� ������, ��ġ�� ���ؼ� �׷��� �������� �����ϴ� ����.

	LPD3DXSPRITE		m_pSprite;		//ID3DXSprite�� ������, ���̷�Ʈ �󿡼� 2D�ؽ��� �������� �����ϴ� COM��ü.
	LPD3DXFONT			m_pFont;		//iD3DXFont�� ������
	LPD3DXLINE			m_pLine;		
	//���⼭ I�� Interface�� ����
	//�̷��� I�� �����ϴ� �ڷ����� COM��ü ��� �θ���.(COM: Component Object Model)

	//�����ڿ� �Ҹ���
private:
	explicit CDeviceManager();
	virtual ~CDeviceManager();

	//Get �Լ�
public:
	LPDIRECT3DDEVICE9 GetDevice(){ return m_pDevice; }
	LPD3DXSPRITE GetSprite(){ return m_pSprite; }
	LPD3DXFONT GetFont(){ return m_pFont; }
	LPD3DXLINE GetLine(){ return m_pLine; }

private:
	void Release();

public:
	HRESULT InitDevice(MODE _eMode);       //��ġ �ʱ�ȭ �Լ�
	void Render_Begin();
	void Render_End(HWND hWnd = nullptr);
};

#define DEVICEMGR CDeviceManager::GetInstance()

#endif // !__DEVICE_MANAGER_H__
