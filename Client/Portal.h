#ifndef __PORTAL_H__
#define __PORTAL_H__

#include "MapObject.h"
class CPortal:
	public CMapObject{
public:
	explicit CPortal();
	virtual ~CPortal();

private:
	void UpdateRect();

	// CMapObject��(��) ���� ��ӵ�
public:
	virtual void Activate() override;

	// CGameObject��(��) ���� ��ӵ�
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
public:
	static CPortal* Create(const D3DXVECTOR3& _vPos);

};

#endif // !__PORTAL_H__

