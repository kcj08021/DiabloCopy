#ifndef __HELMET_H__
#define __HELMET_H__

#include "Item.h"
class CHelmet:
	public CItem{
public:
	explicit CHelmet();
	virtual ~CHelmet();

public:
	virtual void SetIndex(int i){ m_iIndex = i; m_tStat.iArmor = m_iIndex + 1; m_tStat.szName = L"HELMET\nARMOR: " + to_wstring(m_tStat.iArmor);
	}

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
	static CHelmet* Create(const D3DXVECTOR3& _vPos = D3DXVECTOR3{});
};

#endif // !__HELMET_H__
