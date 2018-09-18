#pragma once
#include "GameCore\GameObject\Components\ComponentBase.h"



class AttachTarget : public ComponentBase
{
public:
	AttachTarget();
	~AttachTarget();

	virtual void Update();

	void SetTarget(GameObject* target)
	{
		this->target = target->GetTransform();
	}
private:
	Transform* target;
};