#ifndef __AMULET_H__
#define __AMULET_H__

#include "Item.h"
class CAmulet:
	public CItem{
private:
	explicit CAmulet();
public:
	virtual ~CAmulet();

	// CItem��(��) ���� ��ӵ�
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
public:
	static CAmulet* Create(const D3DXVECTOR3& _vPos = D3DXVECTOR3{});
};

#endif // !__AMULET_H__
