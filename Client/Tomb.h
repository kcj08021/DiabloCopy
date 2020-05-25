#ifndef __TOMB_H__

#include "MapObject.h"

class CObjectManager;

class CTomb:
	public CMapObject{
private:
	CObjectManager* m_pObjMgr;

public:
	explicit CTomb();
	virtual ~CTomb();

private:
	void UpdateRect();
	virtual void Activate() override;

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
public:
	static CTomb* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile);
};


#endif // !__TOMB_H__

