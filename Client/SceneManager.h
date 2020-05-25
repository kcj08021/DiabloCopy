#ifndef __SCENE_MANAGER_H__
#define __SCENE_MANAGER_H__
class CScene;

class CSceneManager{
	DECLARE_SINGLETON(CSceneManager);
public:
	enum SCENE_TYPE{
		SCENE_TITLE, SCENE_WORLD1, SCENE_WORLD2
	};


private:
	CScene* m_CurrScene;
	SCENE_TYPE m_eCurrSceneType;
	SCENE_TYPE m_ePrevSceneType;

private:
	explicit CSceneManager();
	virtual ~CSceneManager();

//¸â¹ö ÇÔ¼ö
public:
	void SetScene(const SCENE_TYPE _enum);

private:
	void Release();
public:
	void Update();
	void LateUpdate();
	void Render();
};

#define SCENEMGR CSceneManager::GetInstance()

#endif // !__SCENE_MANAGER_H__

