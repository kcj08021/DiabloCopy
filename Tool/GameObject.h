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

//Get 멤버 함수
public:
	INFO& GetInfo() { return m_tInfo; }
	bool IsVisible(){ return m_bIsVisible; }

//Set 멤버 함수
public:
	void SetPosition(float _x, float _y){ m_tInfo.vPos.x = _x; m_tInfo.vPos.y = _y; }
	void SetVisible(bool _bool){ m_bIsVisible = _bool; }

//가상 함수
private:
	virtual HRESULT Release() = 0;
protected:
	virtual HRESULT Initialize() = 0;
	virtual HRESULT LateInit();				//모든 오브젝트들의 Initialize()가 호출된 뒤 호출되는 함수
public:
	virtual HRESULT Update() = 0;
	virtual HRESULT LateUpdate() = 0;		//모든 오브젝트들의 Update()가 호출된 뒤 호출되는 함수
	virtual HRESULT Render() = 0;
};
#endif // !__GAMEOBJECT_H__
