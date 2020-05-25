#ifndef __SCENE_H__
#define __SCENE_H__

class CScene {
protected:
	CObjectManager* m_pObjMgr;
public:
	explicit CScene();
	virtual ~CScene();

private:
	virtual HRESULT Release() = 0;
public:		 
	virtual HRESULT Initialize() = 0;
	virtual HRESULT Update() = 0;
	virtual HRESULT LateUpdate() = 0;
	virtual HRESULT Render() = 0;
};

#endif // !__SCEAN_H__



