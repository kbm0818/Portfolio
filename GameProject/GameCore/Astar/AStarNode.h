#pragma once

class AStarNode
{
public:
	AStarNode();
	~AStarNode();

	void Set(UINT x, UINT z, D3DXVECTOR3 position, bool bWalkable);

	float GetFCost()
	{
		fCost = gCost + hCost; 
		return fCost;
	}

	bool operator==(const AStarNode& rhs) const
	{
		return (bool)(position == rhs.position);
	}

	bool operator<(const AStarNode& rhs) const
	{
		return fCost < rhs.fCost;
	}

public:
	AStarNode* parentNode;

	int x, y, z;
	bool bWalkable;
	bool bOpened, bClosed, bPath;
	D3DXVECTOR3 position;

	float gCost, hCost;
	float fCost;
};