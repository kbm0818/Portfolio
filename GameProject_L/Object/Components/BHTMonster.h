#pragma once
#include "BehaviourTree.h"


class GameObject;
class ModelAnimationController;
class Transform_PhysiscC;
struct BHTMonsterAICDataContext;

namespace BHTMonster
{
	class Monster_IDle : public BehaviourTree::Node
	{
	public:
		Monster_IDle(GameObject* object, BHTMonsterAICDataContext* data);
		virtual bool Update() override;

	private:
		BHTMonsterAICDataContext* data;
		ModelAnimationController* animationController;
		Transform_PhysiscC* t;
	};

	class Monster_Follow : public BehaviourTree::Node
	{
	public:
		Monster_Follow(GameObject* object, BHTMonsterAICDataContext* data);
		virtual bool Update() override;

	private:
		BHTMonsterAICDataContext* data;
		ModelAnimationController* animationController;
		Transform_PhysiscC* t;
	};

	class Monster_Attack : public BehaviourTree::Node
	{
	public:
		Monster_Attack(GameObject* object, BHTMonsterAICDataContext* data);
		virtual bool Update() override;

	private:
		BHTMonsterAICDataContext* data;
		ModelAnimationController* animationController;
		Transform_PhysiscC* t;
	};

}