#include "stdafx.h"
#include "AStarManager.h"
#include "ColTile.h"

IMPLEMENT_SINGLETON(CAStarManager)

CAStarManager::CAStarManager(){
}


CAStarManager::~CAStarManager(){
}

HRESULT CAStarManager::PathFinding(list<const D3DXVECTOR3*>& _PathList, const D3DXVECTOR3 & _vSrc, const D3DXVECTOR3 & _vDest, size_t _sizeDepth){
	//맵 없음 경로 탐색 중단
	if(m_pColTile == nullptr)
		return false;

	//경로 초기화
	for(const D3DXVECTOR3*& elem : _PathList){
		SafeDelete(elem);
	}
	_PathList.clear();

	list<AStarNode*> ClosedList;			//경로로 선택 된 노드들의 리스트(닫힌 리스트)
	list<AStarNode*> OpenedList;			//경로로 선택되지 않은 노드들의 리스트(열린 리스트)

											//출발타일과 도착타일 설정
	const COL_INFO* DestTile = m_pColTile->GetTile(_vDest);
	const COL_INFO* CurrTile = m_pColTile->GetTile(_vSrc);
	


	//도착 혹은 출발 타일이 없으면 길찾기 중단
	if(nullptr == DestTile || nullptr == CurrTile)
		return false;

	//출발 타일 바로 옆이 도착 타일이면서 도달 불가능한 경우
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
	//길찾기 시작
	while(ClosedList.back()->pTile != DestTile){
		//탐색 지연시 길찾기 중단
		if(ClosedList.size() > _sizeDepth){
			goto LOOP_EXIT;
		}

		//현재 타일의 주변 타일 정보 생성
		CurrTile = currNode->pTile;
		size_t CurrIndex = m_pColTile->GetIndex(CurrTile->vPos);
		for(int i = -TILEX * 2; i <= TILEX * 2; i += TILEX * 2){
			for(int j = -1; j <= 1; ++j){
				//현재 타일인 경우 정보생성 건너뜀
				if(0 == i && 0 == j)
					continue;

				//대각선으로 이동하는 것에 대한 보정
				if(i == TILEX * 2 || i == -TILEX * 2)
					if(m_pColTile->GetTile(CurrIndex + j) == nullptr
						|| m_pColTile->GetTile(CurrIndex + j)->bIsColi ==false)
						continue;

				if(j == -1 || j == 1)
					if(m_pColTile->GetTile(CurrIndex + i) == nullptr ||
						m_pColTile->GetTile(CurrIndex + i)->bIsColi == false)
						continue;

				//타일 정보 불러오기
				const COL_INFO* selectTile = m_pColTile->GetTile(CurrIndex + i + j);

				//해당 타일이 이미 이동경로에 포함되어 있는 경우 건너뜀
				for(AStarNode* elem : ClosedList)
					if(elem->pTile == selectTile){
						selectTile = nullptr;
						break;
					}

				//타일 정보 없을때 건너뜀
				if(nullptr == selectTile)
					continue;

				//해당 타일이 이동할 수 없는 타일인 경우 건너뜀
				if(false == selectTile->bIsColi)
					if(selectTile == DestTile)
						//탐색 중에 선택된 타일이 종착지이면서 종착지가 이동 불가인 경우 탐색루프 탈출
						goto LOOP_EXIT;
					else
						continue;

				//이동하는 비용
				float fG = 0.f;
				if(i == 0 || j == 0) fG = currNode->iCostG + ASTAR_DIAG_G;
				else if(i * j > 0) fG = currNode->iCostG + ASTAR_VERT_G;
				else fG =  currNode->iCostG + ASTAR_HORI_G;

				//도착점 까지의 거리
				float fH = abs(DestTile->vPos.x - selectTile->vPos.x) + abs(DestTile->vPos.y - selectTile->vPos.y);

				//A* 노드 생성
				AStarNode* Node = new AStarNode{ selectTile, fG, fH, fG + fH, currNode };

				//열린 리스트에서 현재 타일을 가진 노드가 있는 경우 이동비용 비교후 적은 것으로 교체
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

				//열린 리스트에 노드 추가
				if(nullptr != Node)
					OpenedList.push_back(Node);
			}
		}


		//열린 경로가 더 이상 없는 경우 == 경로 없음
		if(OpenedList.empty()){
			break;
		}

		//열린 리스트를 F값으로 정렬(오름차순)
		OpenedList.sort([](tagAStarNode* _dest, tagAStarNode* _src)-> bool{
			return _dest->iCostF < _src->iCostF;
		}
		);

		//F값이 제일 적은 노드를 열린 리스트에서 제거하고, 닫힌 리스트에 추가
		currNode = OpenedList.front();
		ClosedList.push_back(currNode);

		OpenedList.pop_front();
	}//경로탐색 종료

	///////////////////////////////////////////////////////////////
LOOP_EXIT:		//이중 for문과 While문의 탈출 지점(goto)

	 //경로 탐색 종료후, currNode는 도착점을 가리킴
	while(nullptr != currNode){
		//노드의 타일 정보로 이동경로 설정 후, 이동경로 리스트의 앞에 추가
		const COL_INFO* tile = currNode->pTile;
		D3DXVECTOR3* pt = new D3DXVECTOR3(tile->vPos);

		_PathList.push_front(pt);

		//부모 노드로 변경후, 부모가 nullptr인 경우까지 반복(부모가 nullptr인 경우 출발점이 됨)
		currNode = currNode->pParantNode;
	}

	//출발점 보정(타일 중심이 아닌, 현재 위치로 재설정함)
	SafeDelete(_PathList.front());
	_PathList.pop_front();

	//경로 탐색 완료, 열린 리스트와 닫힌 리스트 정리
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
