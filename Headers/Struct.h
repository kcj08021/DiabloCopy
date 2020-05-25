#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo{
	D3DXVECTOR3	vPos;	// 위치벡터
	D3DXVECTOR3 vDir;	// 방향벡터
	D3DXVECTOR3 vLook;	// 기준벡터
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld; // 월드행렬 = 크기행렬 * 자전행렬 * 이동행렬 * 공전행렬 * 부모행렬
}INFO;

typedef struct tagStatus{
	wstring szName;		//캐릭터 이름
	wstring szJob;		//캐릭터 직업

	byte iLevel		=0;		//현재 레벨
					
	int iNextExp	=0;		//다음 레벨까지 필요한 경험치
	int iExp		=0;		//현재 경험치
					
	byte iStrength	=0;		//근력
	byte iDexterity	=0;		//민첩
	byte iMagic		=0;		//마력
	byte iVitality	=0;		//활력

	int iDamage		=0;		//데미지.
	int iArmor		=0;		//방어력

					
	int iMaxHp		=0;		//체력 최대치
	int iHp			=0;		//현재 체력
	int iMaxMp		=0;		//마력 최대치
	int iMp			=0;		//현재 마력
}STAT_INFO;

typedef struct tagTextrueInfo{
//	enum ANI_TYPE{ANI_NONE, ANI_END, ANI_LOOP};
	LPDIRECT3DTEXTURE9 pTexture;
	D3DXIMAGE_INFO tImgInfo;
	D3DXVECTOR3 vec3CenterPoint;
	int iImageCount;
	wstring wstrFilePath;
//	ANI_TYPE eAniType;
}TEXTURE_INFO;

typedef struct tagTileInfo{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	BYTE byDrawId;
	BYTE byDrawOption;
}TILE_INFO;

typedef struct tagColTile{
	D3DXVECTOR3 vPos;
	D3DXVECTOR3 vSize;
	bool bIsColi;
}COL_INFO;

typedef struct tagAStarNode {
	TILE_INFO* pTile;
	float iCostG;
	float iCostH;
	float iCostF;
	tagAStarNode* pParantNode;
}AStarNode;

typedef struct tagPathInfo{
	wstring wstrTypeKey	= L"";
	wstring wstrObjectKey = L"";
	size_t	iImgCount = 0;
	wstring wstrStateKey = L"";
	wstring wstrDirectionKey = L"";
	BYTE	iColorA = 0;
	BYTE	iColorR = 0;
	BYTE	iColorG = 0;
	BYTE	iColorB = 0;
	int		iCenterX = 0;
	int		iCenterY = 0;
	wstring wstrImgPath = L"";
}PATH_INFO;

#endif // !__STRUCT_H__
