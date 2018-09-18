#include "stdafx.h"
#include "AStarNode.h"

AStarNode::AStarNode()
	:x(0), y(0), z(0), bWalkable(false), position(D3DXVECTOR3(0,0,0)), parentNode(nullptr)
	, gCost(0), hCost(0), fCost(0), bOpened(false), bClosed(false), bPath(false)
{
}

AStarNode::~AStarNode()
{
}

void AStarNode::Set(UINT x, UINT z, D3DXVECTOR3 position, bool bWalkable)
{
	this->x = x;
	this->z = z;
	this->position = position;
	this->bWalkable = bWalkable;
}
