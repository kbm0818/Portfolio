#pragma once
#include "TransformC.h"

class Transform_PhysiscC : public TransformC
{
public:
	Transform_PhysiscC(GameObject* object);
	virtual ~Transform_PhysiscC();

	virtual void Update(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);

	void GetVelocity(D3DXVECTOR3& vel) { vel = velocity; }
	void SetVelocity(D3DXVECTOR3& vel){velocity = vel;}

	D3DXVECTOR3& GetVelocity2() { return velocity; }

	void Jump();

protected:
	const float Gravity = -300.0f;
	const float JumpPower = 50.0f;

	D3DXVECTOR3 velocity;
	D3DXVECTOR3 forward;
	D3DXVECTOR3 right;

	D3DXVECTOR3 prePosition;

	bool isJump;

	GameObject* object;
};