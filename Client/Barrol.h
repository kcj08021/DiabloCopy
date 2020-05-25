#ifndef __BARROL_H__
#define __BARROL_H__

#include "GameObject.h"

class CColTile;
class CPlayer;

class CBarrol:
	public CGameObject{
	CPlayer* m_pPlayer;
	CColTile* m_pColTile;
	bool m_bIsItemDroped;
public:
	explicit CBarrol();
	virtual ~CBarrol();

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
	static CBarrol* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile);
};



#endif // !__BARROL_H__
