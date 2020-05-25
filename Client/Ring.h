#ifndef __RING_H__
#define __RING_H__

#include "Item.h"
class CRing:
	public CItem{
private:
	explicit CRing();
public:
	virtual ~CRing();

	// CItem을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
public:
	static CRing* Create(const D3DXVECTOR3& _vPos = D3DXVECTOR3{});
};

#endif // !__RING_H__
