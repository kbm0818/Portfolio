#pragma once

struct BHTMonsterAICDataContext
{
	D3DXVECTOR3 direction;
	float distance;

	float followD = 10.0f;
	float attackD = 1.0f;

	float MoveSpeed = 10.0f;
	float TurnSpeed = 3.0f;

	void SetFollowD(float d) { followD = d; }
	void SetAttackD(float d) { attackD = d; }
};

class Transform_PhysiscC;
class BHTMonsterAIC : public IComponent
{
public:
	BHTMonsterAIC(GameObject* object);
	~BHTMonsterAIC();

	virtual void Update(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);
	virtual void DepthRender(GlobalValues* exValue);

	BHTMonsterAICDataContext*& GetDataContext() { return data; }

private:
	GameObject* object;
	Transform_PhysiscC* t;
	class BehaviourTree* bht;
	BHTMonsterAICDataContext* data;
};
