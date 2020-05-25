#ifndef __ASTAR_MANAGER_H__
#define __ASTAR_MANAGER_H__

class CColTile;

class CAStarManager{

private:
	typedef struct tagAStarNode{
		const COL_INFO* pTile;
		float iCostG;
		float iCostH;
		float iCostF;
		tagAStarNode* pParantNode;
	}AStarNode;		//A* 노드
	enum{ 
		ASTAR_HORI_G = TILECX, 
		ASTAR_VERT_G = TILECY, 
		ASTAR_DIAG_G = 143, 
		DEFAULT_ASTAR_DEPTH_MAX = 200 
	};										//상수들

DECLARE_SINGLETON(CAStarManager)

private:
	CColTile* m_pColTile;

private:
	explicit CAStarManager();
	virtual ~CAStarManager();

public:
	void SetColTile(CColTile* _pColTile){ m_pColTile = _pColTile; }

public:
	HRESULT PathFinding(list<const D3DXVECTOR3*>& _PathList, const D3DXVECTOR3& _vSrc, const D3DXVECTOR3& _vDest, size_t _sizeDepth = DEFAULT_ASTAR_DEPTH_MAX);
};

#define ASTARMGR CAStarManager::GetInstance()
#endif // !__ASTAR_MANAGER_H__
