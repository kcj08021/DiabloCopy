#ifndef __OBJECT_MANAGER_H__
#define __OBJECT_MANAGER_H__

class CGameObject;

typedef list<CGameObject*> OBJ_LIST;

class CObjectManager{
	DECLARE_SINGLETON(CObjectManager)
public:
	enum OBJ_TYPE{
		OBJ_BACKGROUND, OBJ_PLAYER, OBJ_MONSTER, OBJ_ITEM, OBJ_MAPOBJ, OBJ_EFFECT, OBJ_OTHER, OBJ_UI, OBJ_END
	};
private:
	enum RENDER_LAYER{
		RENDER_BACKGROUND, RENDER_DEADOBJ, RENDER_ITEM, RENDER_GAMEOBJ, RENDER_EFFECT, RENDER_OTHER, RENDER_UI, RENDER_END
	};
private:
	list<CGameObject*> m_ObjList[OBJ_END];
	list<CGameObject*> m_RenderList[RENDER_END];

private:
	explicit CObjectManager();
	virtual ~CObjectManager();

public:
	OBJ_LIST& GetList(const OBJ_TYPE _enum);
	CGameObject* GetPlayer();

public:
	void AddObject(OBJ_TYPE _enum, CGameObject* _pObj);

private:
	void Release();
public:
	int Update();
	void LateUpdate();
	void Render();
};

#define OBJMGR CObjectManager::GetInstance()

#endif // !__OBJECT_MANAGER_H__



