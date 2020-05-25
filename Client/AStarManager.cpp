#include "stdafx.h"
#include "AStarManager.h"
#include "ColTile.h"

IMPLEMENT_SINGLETON(CAStarManager)

CAStarManager::CAStarManager(){
}


CAStarManager::~CAStarManager(){
}

HRESULT CAStarManager::PathFinding(list<const D3DXVECTOR3*>& _PathList, const D3DXVECTOR3 & _vSrc, const D3DXVECTOR3 & _vDest, size_t _sizeDepth){
	//�� ���� ��� Ž�� �ߴ�
	if(m_pColTile == nullptr)
		return false;

	//��� �ʱ�ȭ
	for(const D3DXVECTOR3*& elem : _PathList){
		SafeDelete(elem);
	}
	_PathList.clear();

	list<AStarNode*> ClosedList;			//��η� ���� �� ������ ����Ʈ(���� ����Ʈ)
	list<AStarNode*> OpenedList;			//��η� ���õ��� ���� ������ ����Ʈ(���� ����Ʈ)

											//���Ÿ�ϰ� ����Ÿ�� ����
	const COL_INFO* DestTile = m_pColTile->GetTile(_vDest);
	const COL_INFO* CurrTile = m_pColTile->GetTile(_vSrc);
	


	//���� Ȥ�� ��� Ÿ���� ������ ��ã�� �ߴ�
	if(nullptr == DestTile || nullptr == CurrTile)
		return false;

	//��� Ÿ�� �ٷ� ���� ���� Ÿ���̸鼭 ���� �Ұ����� ���
	if(!DestTile->bIsColi){
		size_t CurrIndex = m_pColTile->GetIndex(CurrTile->vPos);
		for(int i = -TILEX * 2; i <= TILEX * 2; i += TILEX * 2){
			for(int j = -1; j <= 1; ++j){
				if(DestTile == m_pColTile->GetTile(CurrIndex + i + j))
					return S_OK;
			}
		}
	}

	AStarNode* currNode = new AStarNode{ CurrTile, 0, 0, 0, nullptr };
	ClosedList.push_back(currNode);

	///////////////////////////////////////////////////////////////
	//��ã�� ����
	while(ClosedList.back()->pTile != DestTile){
		//Ž�� ������ ��ã�� �ߴ�
		if(ClosedList.size() > _sizeDepth){
			goto LOOP_EXIT;
		}

		//���� Ÿ���� �ֺ� Ÿ�� ���� ����
		CurrTile = currNode->pTile;
		size_t CurrIndex = m_pColTile->GetIndex(CurrTile->vPos);
		for(int i = -TILEX * 2; i <= TILEX * 2; i += TILEX * 2){
			for(int j = -1; j <= 1; ++j){
				//���� Ÿ���� ��� �������� �ǳʶ�
				if(0 == i && 0 == j)
					continue;

				//�밢������ �̵��ϴ� �Ϳ� ���� ����
				if(i == TILEX * 2 || i == -TILEX * 2)
					if(m_pColTile->GetTile(CurrIndex + j) == nullptr
						|| m_pColTile->GetTile(CurrIndex + j)->bIsColi ==false)
						continue;

				if(j == -1 || j == 1)
					if(m_pColTile->GetTile(CurrIndex + i) == nullptr ||
						m_pColTile->GetTile(CurrIndex + i)->bIsColi == false)
						continue;

				//Ÿ�� ���� �ҷ�����
				const COL_INFO* selectTile = m_pColTile->GetTile(CurrIndex + i + j);

				//�ش� Ÿ���� �̹� �̵���ο� ���ԵǾ� �ִ� ��� �ǳʶ�
				for(AStarNode* elem : ClosedList)
					if(elem->pTile == selectTile){
						selectTile = nullptr;
						break;
					}

				//Ÿ�� ���� ������ �ǳʶ�
				if(nullptr == selectTile)
					continue;

				//�ش� Ÿ���� �̵��� �� ���� Ÿ���� ��� �ǳʶ�
				if(false == selectTile->bIsColi)
					if(selectTile == DestTile)
						//Ž�� �߿� ���õ� Ÿ���� �������̸鼭 �������� �̵� �Ұ��� ��� Ž������ Ż��
						goto LOOP_EXIT;
					else
						continue;

				//�̵��ϴ� ���
				float fG = 0.f;
				if(i == 0 || j == 0) fG = currNode->iCostG + ASTAR_DIAG_G;
				else if(i * j > 0) fG = currNode->iCostG + ASTAR_VERT_G;
				else fG =  currNode->iCostG + ASTAR_HORI_G;

				//������ ������ �Ÿ�
				float fH = abs(DestTile->vPos.x - selectTile->vPos.x) + abs(DestTile->vPos.y - selectTile->vPos.y);

				//A* ��� ����
				AStarNode* Node = new AStarNode{ selectTile, fG, fH, fG + fH, currNode };

				//���� ����Ʈ���� ���� Ÿ���� ���� ��尡 �ִ� ��� �̵���� ���� ���� ������ ��ü
				for(AStarNode*& elem : OpenedList){
					if(elem->pTile == Node->pTile)
						if(elem->iCostG > Node->iCostG){
							SafeDelete(elem);
							elem = Node;
							Node = nullptr;
							break;
						} else{
							SafeDelete(Node);
							break;
						}
				}

				//���� ����Ʈ�� ��� �߰�
				if(nullptr != Node)
					OpenedList.push_back(Node);
			}
		}


		//���� ��ΰ� �� �̻� ���� ��� == ��� ����
		if(OpenedList.empty()){
			break;
		}

		//���� ����Ʈ�� F������ ����(��������)
		OpenedList.sort([](tagAStarNode* _dest, tagAStarNode* _src)-> bool{
			return _dest->iCostF < _src->iCostF;
		}
		);

		//F���� ���� ���� ��带 ���� ����Ʈ���� �����ϰ�, ���� ����Ʈ�� �߰�
		currNode = OpenedList.front();
		ClosedList.push_back(currNode);

		OpenedList.pop_front();
	}//���Ž�� ����

	///////////////////////////////////////////////////////////////
LOOP_EXIT:		//���� for���� While���� Ż�� ����(goto)

	 //��� Ž�� ������, currNode�� �������� ����Ŵ
	while(nullptr != currNode){
		//����� Ÿ�� ������ �̵���� ���� ��, �̵���� ����Ʈ�� �տ� �߰�
		const COL_INFO* tile = currNode->pTile;
		D3DXVECTOR3* pt = new D3DXVECTOR3(tile->vPos);

		_PathList.push_front(pt);

		//�θ� ���� ������, �θ� nullptr�� ������ �ݺ�(�θ� nullptr�� ��� ������� ��)
		currNode = currNode->pParantNode;
	}

	//����� ����(Ÿ�� �߽��� �ƴ�, ���� ��ġ�� �缳����)
	SafeDelete(_PathList.front());
	_PathList.pop_front();

	//��� Ž�� �Ϸ�, ���� ����Ʈ�� ���� ����Ʈ ����
	for(AStarNode* elem : OpenedList){
		SafeDelete(elem);
	}
	OpenedList.clear();

	for(AStarNode* elem : ClosedList){
		SafeDelete(elem);
	}
	ClosedList.clear();
	
	return S_OK;
}
