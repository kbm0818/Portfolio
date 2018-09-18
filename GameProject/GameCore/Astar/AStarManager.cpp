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

	while (openSet.size() > 0) //���¼� ���鼭 fCost �� �������� ��� ã��
	{
		AStarNode* node = openSet.front();

		for (UINT i = 1; i < openSet.size(); i++)
		{
			if (openSet[i]->GetFCost() < node->GetFCost() || openSet[i]->GetFCost() == node->GetFCost()) //���� ���� ��庸�� f���� �۰ų� ����
			{
				if (openSet[i]->hCost < node->hCost)
					node = openSet[i];
			}
		}

		if (IsContain(openSet, findIter, node)) //���¼¿� ��尡 ������ �����ϰ� �����¿��߰�
		{
			openSet.erase(findIter);
			closedSet.push_back(node);
			node->bOpened = false;
			node->bClosed = true;
		}

		if (node == targetNode) //��尡 Ÿ���̶�� �н��� �����ϰ� ����
		{
			RetracePath(startNode, targetNode);
			return grid->pathNodes;
		}
		vector<AStarNode*> neighbours = grid->GetNeighbours(*node);
		for each(AStarNode* neighbour in neighbours) //���� 8���� ���ã��
		{
			bool bFind = IsContain(closedSet, findIter, neighbour); //�����¿� �ִ���Ȯ��

			if (neighbour->bWalkable == false || bFind == true) //�����ų� �����¿������� ���� ���
				continue;

			float newCostToNeighbour = node->gCost + GetDistance(node, neighbour); //������� gCost�� �� gCost + �Ÿ�

			bFind = IsContain(openSet, findIter, neighbour); //����尡 ���¼¿��ִ��� Ȯ��

			if (newCostToNeighbour < neighbour->gCost || bFind == false) // �� ����� ������뺸�� �۰ų� ���¼¿� ������
			{
				//�� ����� ��� �ٽ� ����� �θ��� �ڽ����� ����
				neighbour->gCost = newCostToNeighbour;
				neighbour->hCost = GetDistance(neighbour, targetNode);
				neighbour->parentNode = node;

				if (bFind == false) //���¼¿� ������ ���¼¿��߰�
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
	reverse(pathNodes.begin(), pathNodes.end()); //������
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
