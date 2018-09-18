#include "stdafx.h"
#include "AStarUnit.h"
#include "AStarNode.h"
#include "AStarManager.h"

AStarUnit::AStarUnit()
	:nextIndex(0), nextNode(nullptr), model(nullptr), speed(50.0f)
	, currentPos(0,0,0), nextPos(0,0,0)
{
	astarMgr = AStarManager::Get();
}

AStarUnit::~AStarUnit()
{
}

void AStarUnit::Update()
{
	if (model == nullptr)
		return;

	if (Keyboard::Get()->Down('M'))
	{		
		D3DXVECTOR3 target = astarMgr->mousePos;
		model->GetTransform(s, r, t);

		pathNodes = astarMgr->FindPath(t, target);
		MovePath();
	}

	if (pathNodes.size() > 0)
	{
		model->GetTransform(s, r, t);
		currentPos = t;
		nextPos = nextNode->position;

		D3DXVECTOR3 distVec = nextPos - currentPos;
		D3DXVECTOR3 dir;
		D3DXVec3Normalize(&dir, &distVec);
		
		currentPos += dir*speed*Time::Delta();
		float distance = D3DXVec3LengthSq(&distVec);
		float rot = atan2(dir.z, dir.x);
		r.y = 90 - Math::ToDegree(rot);
		if (distance < 0.5f)
		{
			nextIndex++;
			if (nextIndex > pathNodes.size() - 1)
			{
				pathNodes.clear();
				return;
			}
			nextNode = pathNodes[nextIndex];
		}
		model->SetTransform(s, r, currentPos);
	}
}

void AStarUnit::MovePath()
{
	nextIndex = 0;
	nextNode = nullptr;

	nextNode = pathNodes[nextIndex];
}

