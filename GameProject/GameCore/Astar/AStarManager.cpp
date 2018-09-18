#include "stdafx.h"
#include "AStarManager.h"
#include "AStarGrid.h"
#include "AStarNode.h"
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <queue>

AStarManager* AStarManager::instance = nullptr;
AStarManager * AStarManager::Get()
{
	if (instance == nullptr)
		instance = new AStarManager();
	return instance;
}

void AStarManager::Delete()
{
	SAFE_DELETE(instance);
}

AStarManager::AStarManager()
	:grid(nullptr), mousePos(0,0,0)
{
}

AStarManager::~AStarManager()
{
	SAFE_DELETE(grid);
}

void AStarManager::CreateGrid(D3DXVECTOR2 terrainSize, int gridSizeX, int gridSizeZ)
{
	SAFE_DELETE(grid);
	grid = new AStarGrid(terrainSize, gridSizeX, gridSizeZ);
}

vector<AStarNode*>& AStarManager::FindPath(D3DXVECTOR3 startPos, D3DXVECTOR3 targetPos)
{
	grid->ResetNodes();
	AStarNode* startNode = grid->NodeFromWorldPoint(startPos);
	AStarNode* targetNode = grid->NodeFromWorldPoint(targetPos);
	
	startNode->bWalkable = true;
	targetNode->bWalkable = true;

	vector<AStarNode*> openSet;
	vector<AStarNode*> closedSet;
	vector<AStarNode*> ::iterator findIter;

	startNode->bOpened = true;
	openSet.push_back(startNode);

	while (openSet.size() > 0) //오픈셋 돌면서 fCost 가 가장작은 노드 찾기
	{
		AStarNode* node = openSet.front();

		for (UINT i = 1; i < openSet.size(); i++)
		{
			if (openSet[i]->GetFCost() < node->GetFCost() || openSet[i]->GetFCost() == node->GetFCost()) //먼저 꺼낸 노드보다 f값이 작거나 같음
			{
				if (openSet[i]->hCost < node->hCost)
					node = openSet[i];
			}
		}

		if (IsContain(openSet, findIter, node)) //오픈셋에 노드가 있으면 제거하고 닫힌셋에추가
		{
			openSet.erase(findIter);
			closedSet.push_back(node);
			node->bOpened = false;
			node->bClosed = true;
		}

		if (node == targetNode) //노드가 타겟이라면 패스를 추적하고 종료
		{
			RetracePath(startNode, targetNode);
			return grid->pathNodes;
		}
		vector<AStarNode*> neighbours = grid->GetNeighbours(*node);
		for each(AStarNode* neighbour in neighbours) //주위 8방향 노드찾기
		{
			bool bFind = IsContain(closedSet, findIter, neighbour); //닫힌셋에 있는지확인

			if (neighbour->bWalkable == false || bFind == true) //못가거나 닫힌셋에있으면 다음 노드
				continue;

			float newCostToNeighbour = node->gCost + GetDistance(node, neighbour); //옆노드의 gCost는 내 gCost + 거리

			bFind = IsContain(openSet, findIter, neighbour); //옆노드가 오픈셋에있는지 확인

			if (newCostToNeighbour < neighbour->gCost || bFind == false) // 새 비용이 원래비용보다 작거나 오픈셋에 없으면
			{
				//옆 노드의 비용 다시 계산후 부모노드 자신으로 설정
				neighbour->gCost = newCostToNeighbour;
				neighbour->hCost = GetDistance(neighbour, targetNode);
				neighbour->parentNode = node;

				if (bFind == false) //오픈셋에 없으면 오픈셋에추가
				{
					openSet.push_back(neighbour);
					neighbour->bOpened = true;
				}			
			}
		}
	}

	vector<AStarNode*> nullNodes;
	return nullNodes;
}

AStarNode * AStarManager::NodeFromWorldPoint(D3DXVECTOR3 pos)
{
	return grid->NodeFromWorldPoint(pos);
}

bool AStarManager::FinishPathFind()
{
	return grid->bFinishPathFind;
}

float AStarManager::GetDistance(AStarNode * a, AStarNode * b)
{
	D3DXVECTOR3 vec1 = a->position;
	D3DXVECTOR3 vec2 = b->position;
	D3DXVECTOR3 vec3 = vec2 - vec1;
	return (float)D3DXVec3Length(&vec3);
}

void AStarManager::RetracePath(AStarNode * start, AStarNode * end)
{
	vector<AStarNode*> pathNodes;
	AStarNode *currentNode = end;

	while (currentNode != start) {
		currentNode->bPath = true;
		pathNodes.push_back(currentNode);
		currentNode = currentNode->parentNode;
	}
	reverse(pathNodes.begin(), pathNodes.end()); //뒤집기
	grid->pathNodes = pathNodes;
	grid->bFinishPathFind = true;
}

bool AStarManager::IsContain(vector<AStarNode*>& nodes, vector<AStarNode*> ::iterator& findIter, AStarNode * node)
{
	vector<AStarNode*> ::iterator iter;
	for (iter = nodes.begin(); iter != nodes.end(); iter++)
	{
		if (*iter == node)
		{
			findIter = iter;
			return true;
		}
	}

	return false;
}
