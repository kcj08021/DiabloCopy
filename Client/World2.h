#ifndef __WOLRD2_H__
#define __WOLRD2_H__
#endif // !__WOLRD2_H__

#include "Scene.h"
class CWorld2:
	public CScene{
private:
	CScrollManager* m_pScrollMgr;
public:
	explicit CWorld2();
	virtual ~CWorld2();

	// CScene을(를) 통해 상속됨
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

public:
	static CWorld2* Create();
};

