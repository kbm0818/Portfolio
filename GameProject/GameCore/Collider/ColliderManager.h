#pragma once
#include "GameCore\GameObject\Components\ComponentBase.h"

class ModelSkeleton;

#define COLLISION_CYCLE 0.05f
class ColliderManager : public ComponentBase
{
public:
	ColliderManager() : ComponentBase(), skeleton(nullptr) 
	{
	}
	virtual ~ColliderManager();

	virtual void Awake();
	virtual void Start();
	virtual void RegistEvent();
	virtual void Update();
	virtual void LateUpdate();
	virtual void Render();

	void PushCollider(Collider* collider, int connectBone = 0, bool active = true);

private:
	void OnCollisionCheck(CollisionEventArgs& e);
	
	struct ColliderInfo
	{
		int connectedBone;
		bool active;
		bool bSynced;
	};
	void SyncPosition(ColliderInfo& info, Collider* collider);

	ModelSkeleton * skeleton;
	vector<pair<ColliderInfo,Collider*>> colliders;
	float collisionTimer;
};