#ifndef __ITEM_H__
#define __ITEM_H__

#include "GameObject.h"
class CItem:
	public CGameObject{
protected:
	size_t m_iWidth;
	size_t m_iHeight;
	int m_iIndex;
	bool m_bIsField;
	bool m_bIsSoundOut;
protected:
	void UpdateRect();

public:
	virtual void SetIndex(int i){ m_iIndex = i; }
	int GetIndex(){ return m_iIndex; }

public:
	explicit CItem();
	virtual ~CItem();

public:
	void SetSoundInit(){ m_bIsSoundOut = false; }
	void GetItemSize(POINT& _pt){ _pt.x = m_iWidth; _pt.y = m_iHeight; }
};

#endif // !__ITEM_H__

