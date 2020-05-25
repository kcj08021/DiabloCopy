#ifndef __TRAP_BARROL_H__

#include "GameObject.h"
class CColTile;

class CTrapBarrol:
	public CGameObject{
private:
	CColTile* m_pColTile;
	bool m_bIsDamaged;
public:
	explicit CTrapBarrol();
	virtual ~CTrapBarrol();

	// CGameObject을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT LateInit() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

	static CTrapBarrol* Create(const D3DXVECTOR3& _vPos, CColTile* _pColTile);
};



#endif // !__TRAP_BARROL_H__

