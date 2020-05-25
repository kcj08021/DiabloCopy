#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

class CDeviceManager;
class CTextureManager;

class CGameObject{
private:
protected:
	INFO m_tInfo;
	bool m_bIsVisible;
	bool m_bIsInit;

	CDeviceManager* m_pDeviceMgr;
	CTextureManager* m_pTextureMgr;

public:
	explicit CGameObject();
	virtual ~CGameObject();

//Get ��� �Լ�
public:
	INFO& GetInfo() { return m_tInfo; }
	bool IsVisible(){ return m_bIsVisible; }

//Set ��� �Լ�
public:
	void SetPosition(float _x, float _y){ m_tInfo.vPos.x = _x; m_tInfo.vPos.y = _y; }
	void SetVisible(bool _bool){ m_bIsVisible = _bool; }

//���� �Լ�
private:
	virtual HRESULT Release() = 0;
protected:
	virtual HRESULT Initialize() = 0;
	virtual HRESULT LateInit();				//��� ������Ʈ���� Initialize()�� ȣ��� �� ȣ��Ǵ� �Լ�
public:
	virtual HRESULT Update() = 0;
	virtual HRESULT LateUpdate() = 0;		//��� ������Ʈ���� Update()�� ȣ��� �� ȣ��Ǵ� �Լ�
	virtual HRESULT Render() = 0;
};
#endif // !__GAMEOBJECT_H__
