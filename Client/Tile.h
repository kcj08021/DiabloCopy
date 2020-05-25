#ifndef __TILE_H__
#define __TILE_H__

#include "GameObject.h"
class CTile:
	public CGameObject{


public:
	explicit CTile();
	virtual ~CTile();

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
	static CTile* Create(const D3DXVECTOR3& _vPos, int _iDrawID);
};


#endif // !__TILE_H__
