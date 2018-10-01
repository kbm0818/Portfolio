#include "stdafx.h"
#include "BHTMonster.h"
#include "BHTMonsterAIC.h"
#include "../GameObject.h"
#include "Object\Model\Model.h"
#include "Object\Model\ModelAnimationController.h"
#include "Transform_PhysiscC.h"

BHTMonster::Monster_IDle::Monster_IDle(GameObject* object, BHTMonsterAICDataContext * data)
	:data(data), animationController(nullptr), t(nullptr)
{
	if (object != nullptr)
	{
		Model* m = object->GetMesh<Model>();
		if (m != nullptr)
			animationController = m->GetAnimationController();
		t = object->GetComponent<Transform_PhysiscC>(ComponentFamilyID::TransformComponent);
	}
}

bool BHTMonster::Monster_IDle::Update()
{
	if (data->distance > data->followD)
	{
	/*	D3DXVECTOR3 direction;
		float distance = TransformC::GetDistance(t->GetPosition(), t->GetDefaultPosition(), direction);
		t->SetRotation(direction);

		if (distance > 0.1f)
		{
			t->Move_Forward(data->MoveSpeed);

			if (animationController)
				animationController->SetCurrentAnimation(1);
		}
		else
		{
			if (animationController)
				animationController->SetCurrentAnimation(0);
		}*/
	
		return true;
	}
	else
		return false;
		
}

BHTMonster::Monster_Follow::Monster_Follow(GameObject * object, BHTMonsterAICDataContext * data)
	:data(data), animationController(nullptr), t(nullptr)
{
	if (object != nullptr)
	{
		Model* m = object->GetMesh<Model>();
		if (m != nullptr)
			animationController = m->GetAnimationController();
		t = object->GetComponent<Transform_PhysiscC>(ComponentFamilyID::TransformComponent);
	}
}

bool BHTMonster::Monster_Follow::Update()
{
	if (data->distance > data->attackD)
	{
		t->SetRotation(data->direction);
		t->Move_Forward(data->MoveSpeed);

		if (animationController)
			animationController->SetCurrentAnimation(1);

		return true;
	}
	else
		return false;
}

BHTMonster::Monster_Attack::Monster_Attack(GameObject * object, BHTMonsterAICDataContext * data)
	:data(data), animationController(nullptr), t(nullptr)
{
	if (object != nullptr)
	{
		Model* m = object->GetMesh<Model>();
		if (m != nullptr)
			animationController = m->GetAnimationController();
		t = object->GetComponent<Transform_PhysiscC>(ComponentFamilyID::TransformComponent);
	}
}

bool BHTMonster::Monster_Attack::Update()
{
	//아닌 경우가 생길까?
	t->SetRotation(data->direction);

	animationController->SetCurrentAnimation(2);

	return true;
}
