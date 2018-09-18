#pragma once

class AStarNode;
class AStarGrid
{
public:
	AStarGrid(D3DXVECTOR2 gridWorldSize, int gridSizeX, int gridSizeZ);
	~AStarGrid();

	AStarNode* NodeFromWorldPoint(D3DXVECTOR3 pos);
	vector<AStarNode*> GetNeighbours(const AStarNode& node);
	void ResetNodes();
	vector<AStarNode*> pathNodes;

public:
	bool bFinishPathFind;
private:
	void CreateGrid();

private:
	D3DXVECTOR2 gridWorldSize;
	int gridSizeX, gridSizeZ;
	int gridCountX, gridCountZ;

	AStarNode** nodes; //2차원 배열

};