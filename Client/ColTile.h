#ifndef __COL_TILE_H__
#define __COL_TILE_H__
#include "GameObject.h"

class CDeviceManager;
class CScrollManager;

class CColTile:
	public CGameObject{
private:
	vector<COL_INFO*> m_vecTile;				//�浹 Ÿ��
	vector<int> m_vecDamageTile;				//������ Ÿ��
	vector<const CGameObject*> m_vecPreem;		//���� Ȯ�ο� ��ü
	vector<BYTE> m_vecShadow;

	//�Ŵ��� ����
	CDeviceManager* m_pDeviceMgr;		//��ġ�Ŵ���
	CScrollManager* m_pScrollMgr;		//��ũ�ѸŴ���

private:
	explicit CColTile();
public:
	virtual ~CColTile();

public:
	int GetIndex(const D3DXVECTOR3& _vPos) const;
	const COL_INFO* GetTile(size_t _index) const;
	const COL_INFO* GetTile(const D3DXVECTOR3& _vPos) const;
	const COL_INFO* GetTile(const float _fX, const float _fY) const;
	const int GetDamage(const size_t _index) const;
	const int GetDamage(const D3DXVECTOR3& _vPos) const;
	const CGameObject* GetPreemTile(const D3DXVECTOR3& _vPos) const;
	BYTE GetShadow(const D3DXVECTOR3& _vPos);

public:
	void SetColl(const size_t _index, const bool _bIsColl);
	void SetDamageTile(const size_t _index, const int _iDamage);
	void SetPreemTile(const D3DXVECTOR3& _vPos, const CGameObject * _pObj);
	void SetShadow(const D3DXVECTOR3& _vPos, int _iRange);

public:
	void SaveTile(const TCHAR* _szFilePath);
	void LoadTile(const TCHAR* _szFilePath);

	// CGameObject��(��) ���� ��ӵ�
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

	static CColTile* Create();
};

#endif // !__COL_TILE_H__

