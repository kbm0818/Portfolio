#pragma once

namespace Model
{
	class BinModel;
}

class AStarNode;
class AStarManager;
class AStarUnit
{
public:
	AStarUnit();
	~AStarUnit();

	void Update();

private:
	void MovePath();
public:
	vector<AStarNode*> pathNodes;
	AStarNode* nextNode;
	int nextIndex;
	Model::BinModel* model;

	D3DXVECTOR3 s, r, t;
	D3DXVECTOR3 currentPos;
	D3DXVECTOR3 nextPos;
private:
	AStarManager* astarMgr;
	float speed;
};