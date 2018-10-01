#include "stdafx.h"
#include "BHTMonsterAIC.h"

#include "../GameObject.h"
#include "TransformC.h"
#include "Transform_PhysiscC.h"
#include "BehaviourTree.h"
#include "BHTMonster.h"

BHTMonsterAIC::BHTMonsterAIC(GameObject* object)
	: object(object)
{
	t = object->GetComponent<Transform_PhysiscC>(ComponentFamilyID::TransformComponent);

	bht = new BehaviourTree;
	data = new BHTMonsterAICDataContext;

	BehaviourTree::Sequence* sequence = new BehaviourTree::Sequence;
	BehaviourTree::Selector* selector = new BehaviourTree::Selector;
	BHTMonster::Monster_Attack* attackNode = new BHTMonster::Monster_Attack(object, data);
	BHTMonster::Monster_IDle* idleNode = new BHTMonster::Monster_IDle(object, data);
	BHTMonster::Monster_Follow* followNode = new BHTMonster::Monster_Follow(object, data);

	bht->setRootChild(sequence);
	sequence->AddChild(selector);
	//selector->AddChildren({ idleNode, followNode , attackNode });
	selector->AddChildren({ idleNode, idleNode , idleNode });

}

BHTMonsterAIC::~BHTMonsterAIC()
{
	SAFE_DELETE(bht);
	SAFE_DELETE(data);
}

void BHTMonsterAIC::Update(GlobalValues * exValue)
{
	data->distance = TransformC::GetDistance(t, exValue->Player->GetComponent<TransformC>(ComponentFamilyID::TransformComponent), data->direction);

	bht->Update();
}

void BHTMonsterAIC::Render(GlobalValues * exValue)
{
}

void BHTMonsterAIC::PreRender(GlobalValues * exValue)
{
}

void BHTMonsterAIC::PostRender(GlobalValues * exValue)
{
}

void BHTMonsterAIC::DepthRender(GlobalValues * exValue)
{
}
