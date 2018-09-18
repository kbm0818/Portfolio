#include "stdafx.h"
#include "AStarGrid.h"
#include "AStarNode.h"

AStarGrid::AStarGrid(D3DXVECTOR2 gridWorldSize, int gridSizeX, int gridSizeZ)
	: gridWorldSize(gridWorldSize), gridSizeX(gridSizeX), gridSizeZ(gridSizeZ), gridCountX(0), gridCountZ(0)
	,nodes(nullptr), bFinishPathFind(false)
{
	gridCountX = (int)gridWorldSize.x / gridSizeX;
	gridCountZ = (int)gridWorldSize.y / gridSizeZ;

	CreateGrid();
}

AStarGrid::~AStarGrid()
{
	for (int i = 0; i < gridCountZ; i++)
	{
		SAFE_DELETE_ARRAY(nodes[i]);
	}
	SAFE_DELETE_ARRAY(nodes);
}

void AStarGrid::CreateGrid()
{
	nodes = new AStarNode*[gridCountZ];
	for (int i = 0; i < gridCountZ; i++)
		nodes[i] = new AStarNode[gridCountX];

	for (int z = 0; z < gridCountZ; z++)
	{
		for (int x = 0; x < gridCountX; x++)
		{
			bool bWalkable = true;
			D3DXVECTOR3 worldPosition = D3DXVECTOR3(x * gridSizeX, 0, z* gridSizeZ);
			nodes[z][x].Set(x, z, worldPosition, bWalkable);
			//nodes[z][x].bOpened = true;
		}
	}
}

AStarNode* AStarGrid::NodeFromWorldPoint(D3DXVECTOR3 pos)
{
	float percentX = pos.x / gridWorldSize.x;
	float percentZ = pos.z / gridWorldSize.y;
	int x = (gridCountX - 1) * percentX;
	int z = (gridCountZ - 1) * percentZ;
	return &nodes[z][x];
}

vector<AStarNode*> AStarGrid::GetNeighbours(const AStarNode & node)
{
	vector<AStarNode*> neighbours;

	for (int x = -1; x <= 1; x++) 
	{
		for (int z = -1; z <= 1; z++) 
		{
			if (x == 0 && z == 0)
				continue;

			int checkX = node.x + x;
			int checkZ = node.z + z;

			if (checkX >= 0 && checkX < gridCountX && checkZ >= 0 && checkZ < gridCountZ)
			{
				neighbours.push_back(&nodes[checkZ][checkX]);
			}
		}
	}

	return neighbours;
}

void AStarGrid::ResetNodes()
{
	for (int z = 0; z < gridCountZ; z++)
	{
		for (int x = 0; x < gridCountX; x++)
		{
			nodes[z][x].bClosed = false;
			nodes[z][x].bOpened = false;
			nodes[z][x].bPath = false;
		}
	}
}
