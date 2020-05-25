#ifndef __GOLD_H__
#define __GOLD_H__

#include "Item.h"
class CGold:
	public CItem{
private:
	int m_iGoldCount;

public:
	void AddGold(int _iGold){ m_iGoldCount += _iGold; }
	int GetGold(){ return m_iGoldCount; }

public:
	explicit CGold();
	virtual ~CGold();


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
	static CGold* Create(const D3DXVECTOR3& _vPos,int _iCount);
};

#endif // !__GOLD_H__
