#ifndef __WORLD1_SCENE_H__
#define __WORLD1_SCENE_H__

#include "Scene.h"

class CScrollManager;
class CObjectManager;

class CWorld1:
	public CScene{
private:
	CScrollManager* m_pScrollMgr;

private:
	explicit CWorld1();
public:
	virtual ~CWorld1();

	// CScene을(를) 통해 상속됨
private:
	virtual HRESULT Release() override;
	virtual HRESULT Initialize() override;
public:
	virtual HRESULT Update() override;
	virtual HRESULT LateUpdate() override;
	virtual HRESULT Render() override;

public:
	static CWorld1* Create();
};

#endif // !__WORLD1_SCENE_H__
