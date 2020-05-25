#ifndef __MAP_OBJECT_H__
#define __MAP_OBJECT_H__

#include "GameObject.h"
class CColTile;

class CMapObject:
	public CGameObject{
protected:
	bool m_bIsActivated;
	CColTile* m_pColTile;

public:
	virtual void Activate() = 0;

public:
	explicit CMapObject();
	virtual ~CMapObject();
};

#endif // !__MAP_OBJECT_H__
