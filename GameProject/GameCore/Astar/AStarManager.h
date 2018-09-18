#pragma once

class AStarGrid;
class AStarNode;
class AStarManager
{
public:
	static AStarManager* Get();
	static void Delete();

	void CreateGrid(D3DXVECTOR2 terrainSize, int gridSizeX, int gridSizeZ);
	vector<AStarNode*>& FindPath(D3DXVECTOR3 startPos, D3DXVECTOR3 targetPos);
	AStarNode* NodeFromWorldPoint(D3DXVECTOR3 pos);
	bool FinishPathFind();

	D3DXVECTOR3 mousePos;
private:
	AStarManager();
	~AStarManager();

	float GetDistance(AStarNode* a, AStarNode* b);
	void RetracePath(AStarNode* start, AStarNode* end);
	bool IsContain(vector<AStarNode*>& nodes, vector<AStarNode*> ::iterator& findIter, AStarNode* node);
private:
	static AStarManager* instance;
	AStarGrid* grid;
	
};