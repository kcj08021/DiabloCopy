#ifndef __FUNCTOR_H__
#define __FUNCTOR_H__
#include "Struct.h"
#include "GameObject.h"

class FCostLess{
public:
	bool operator()(AStarNode* _dest, AStarNode* _src){
		return _dest->iCostF < _src->iCostF;
	}
};

class CRenderLess{
public:
	bool operator()(CGameObject* _SrcObj, CGameObject* _DestObj){
	}
};

#endif // !__FUNCTOR_H__


