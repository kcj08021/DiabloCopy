#ifndef __SWORD_H__
#define __SWORD_H__

#include "Item.h"
class CSword:
	public CItem{

private:
	explicit CSword();
public:
	virtual ~CSword();

public:
	virtual void SetIndex(int i){ m_iIndex = i; m_tStat.iDamage = m_iIndex + 1; 	m_tStat.szName = L"SWORD\nDAMAGE: " + to_wstring(m_tStat.iDamage);
	}

	// CItem을(를) 통해 상속됨
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;
	
public:
	static CSword* Create(const D3DXVECTOR3& _vPos = D3DXVECTOR3{});
};

#endif // !__SWORD_H__
