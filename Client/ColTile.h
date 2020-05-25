#ifndef __COL_TILE_H__
#define __COL_TILE_H__
#include "GameObject.h"

class CDeviceManager;
class CScrollManager;

class CColTile:
	public CGameObject{
private:
	vector<COL_INFO*> m_vecTile;				//충돌 타일
	vector<int> m_vecDamageTile;				//데미지 타일
	vector<const CGameObject*> m_vecPreem;		//선점 확인용 객체
	vector<BYTE> m_vecShadow;

	//매니저 변수
	CDeviceManager* m_pDeviceMgr;		//장치매니저
	CScrollManager* m_pScrollMgr;		//스크롤매니저

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

	// CGameObject을(를) 통해 상속됨
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

