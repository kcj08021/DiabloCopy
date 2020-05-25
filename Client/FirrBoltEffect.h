#ifndef __FIREBOLT_EFFECT_H__
#define __FIREBOLT_EFFECT_H__

#include "GameObject.h"
class CObjectManager;
class CColTile;

class CFirrBoltEffect:
	public CGameObject{
private:
	CColTile* m_pColTile;
	CObjectManager* m_pObjMgr;

private:
	explicit CFirrBoltEffect();
public:
	virtual ~CFirrBoltEffect();

private:
	void UpdateAngle();

	// CGameObject��(��) ���� ��ӵ�
private:
	virtual HRESULT Release() override;
	virtual HRESULT LateInit() override;
public:
	virtual HRESULT Initialize() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

public:
	static CFirrBoltEffect* Create(const D3DXVECTOR3& _vPos, const D3DXVECTOR3& _vDir, CColTile* _pColTile);
};

#endif // !__FIREBOLT_EFFECT_H__
