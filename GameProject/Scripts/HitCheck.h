#pragma once
#include "GameCore\GameObject\Components\ComponentBase.h"

class CollisionEventArgs;
class HitCheck : public ComponentBase
{
public:
	HitCheck() : ComponentBase() {}

	virtual void Awake();
	virtual void RegistEvent();
	virtual void Update();

private:
	void OnCollisionEnter(CollisionEventArgs& e);
	void OnCollisionExit(CollisionEventArgs& e);

	vector<Renderer*> renderers;
	float timer;
};