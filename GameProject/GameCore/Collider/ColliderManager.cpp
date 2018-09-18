#include "stdafx.h"
#include "ColliderManager.h"
#include "GameCore\GameObject\IEventArgs.h"

#include "./GameCore/GameObject/Components/Animator/Animator.h"
#include "./Model/Skeleton/ModelSkeleton.h"
#include "Collider.h"

ColliderManager::~ColliderManager()
{
	for (UINT i = 0; i < colliders.size(); ++i)
	{
		SAFE_DELETE(colliders[i].second);
	}
	skeleton = nullptr;
}

void ColliderManager::Awake()
{
	Animator* anim = gameObject->GetComponent<Animator>();
	if(anim)
		skeleton = anim->GetSkeleton();
}

void ColliderManager::Start()
{
	collisionTimer = (float)Math::RandomRange(0, 9) * 0.1f; //0~0.9초만큼의 유예시간을 줌.
}

void ColliderManager::RegistEvent()
{
	gameObject->AddEvent("OnCollisionCheck", name, [this](IEventArgs& e)
	{
		this->OnCollisionCheck(dynamic_cast<CollisionEventArgs&>(e));
	});
}

void ColliderManager::Update()
{
	for (UINT i = 0; i < colliders.size(); ++i)
	{
		colliders[i].first.bSynced = false;
	}
}

void ColliderManager::LateUpdate()
{
	//collisionTimer -= Time::Delta();
	//if (collisionTimer <= 0)
	{
		for (UINT i = 0; i < colliders.size(); ++i)
		{
			if (!colliders[i].first.bSynced)
				SyncPosition(colliders[i].first, colliders[i].second);

			vector<GameObject*> nearList;
			WORLD->GetNearObjects(colliders[i].second->GetWorldPosition(), nearList);

			for (UINT j = 0; j < nearList.size(); ++j)
			{
				if(nearList[j] != gameObject)
					nearList[j]->SendEvent("OnCollisionCheck", CollisionEventArgs(colliders[i].second));
			}

			colliders[i].second->CheckExit();
		}
		//collisionTimer = COLLISION_CYCLE;
	}
}

void ColliderManager::Render()
{
	for (UINT i = 0; i < colliders.size(); ++i)
	{
		if(colliders[i].first.active)
			colliders[i].second->Render();
	}
}

void ColliderManager::PushCollider(Collider * collider, int connectBone, bool active)
{
	ColliderInfo info = { connectBone, active, false };
	collider->SetGameObject(gameObject);
	colliders.push_back(make_pair(info, collider));
}

void ColliderManager::OnCollisionCheck(CollisionEventArgs & e)
{
	for (UINT i = 0; i < colliders.size(); ++i)
	{
		if (!colliders[i].first.active) continue;

		//위치 동기화
		if (!colliders[i].first.bSynced)
		{
			SyncPosition(colliders[i].first, colliders[i].second);
		}

		if (colliders[i].second->IsCollision(e.collider))
		{
			colliders[i].second->Collision(e.collider);
			e.collider->Collision(colliders[i].second);
		}
	}
}

void ColliderManager::SyncPosition(ColliderInfo& info, Collider * collider)
{
	if (skeleton)
	{
		D3DXMATRIX mat = skeleton->GetBoneTransform(info.connectedBone);
		D3DXQUATERNION q;
		D3DXQuaternionRotationMatrix(&q, &mat);

		collider->SetParentPosition(D3DXVECTOR3(mat._41, mat._42, mat._43) + gameObject->GetWorldPosition());
		collider->SetParentRotation(q * gameObject->GetWorldRotation());
		collider->SetParentMatrix(mat * gameObject->GetTransform()->GetWorldMatrix());
	}
	else
	{
		collider->SetParentPosition(gameObject->GetWorldPosition());
		collider->SetParentRotation(gameObject->GetWorldRotation());
		collider->SetParentMatrix(gameObject->GetTransform()->GetWorldMatrix());
	}

	info.bSynced = true;
}
