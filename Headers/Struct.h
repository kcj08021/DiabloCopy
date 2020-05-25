#ifndef __STRUCT_H__
#define __STRUCT_H__

typedef struct tagInfo{
	D3DXVECTOR3	vPos;	// ��ġ����
	D3DXVECTOR3 vDir;	// ���⺤��
	D3DXVECTOR3 vLook;	// ���غ���
	D3DXVECTOR3 vSize;
	D3DXMATRIX matWorld; // ������� = ũ����� * ������� * �̵���� * ������� * �θ����
}INFO;

typedef struct tagStatus{
	wstring szName;		//ĳ���� �̸�
	wstring szJob;		//ĳ���� ����

	byte iLevel		=0;		//���� ����
					
	int iNextExp	=0;		//���� �������� �ʿ��� ����ġ
	int iExp		=0;		//���� ����ġ
					
	byte iStrength	=0;		//�ٷ�
	byte iDexterity	=0;		//��ø
	byte iMagic		=0;		//����
	byte iVitality	=0;		//Ȱ��

	int iDamage		=0;		//������.
	int iArmor		=0;		//����

					
	int iMaxHp		=0;		//ü�� �ִ�ġ
	int iHp			=0;		//���� ü��
	int iMaxMp		=0;		//���� �ִ�ġ
	int iMp			=0;		//���� ����
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
