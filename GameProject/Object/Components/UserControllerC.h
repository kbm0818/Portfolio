#pragma once
#include "Object/Components/IComponent.h"

class GameObject;
class Transform_PhysiscC;
class UserControllerC : public IComponent
{
public:
	UserControllerC(GameObject* object);
	virtual ~UserControllerC();

	virtual void Update(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);

protected:
	void Controll();

protected:
	const float MoveSpeed = 10.0f;
	const float TurnSpeed = 3.0f;
	const float JumpPower = 30.0f;

	float moveSpeed, turnSpeed;

	D3DXVECTOR3 forward;

	GameObject* object;
	Transform_PhysiscC* transformC;

	bool isJump;
};