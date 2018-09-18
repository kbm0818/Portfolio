#include "stdafx.h"
#include "Transform_PhysiscC.h"

#include "Systems\GameSystems\TerrainHeightData.h"


Transform_PhysiscC::Transform_PhysiscC(GameObject* object)
	: prePosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f)), forward(D3DXVECTOR3(0.0f, 0.0f, 1.0f)), right(D3DXVECTOR3(1.0f, 0.0f, 0.0f))
	, velocity(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, isJump(false)
{
}

Transform_PhysiscC::~Transform_PhysiscC()
{
}

void Transform_PhysiscC::Update(GlobalValues * exValue)
{

}

void Transform_PhysiscC::PreRender(GlobalValues * exValue)
{
	float terrainHeight = exValue->TerrainHeightData->GetY(position);

	//회전
	CalcR();

	//이동
	D3DXVec3TransformCoord(&forward, &D3DXVECTOR3(0, 0, 1), &R);
	D3DXVec3TransformCoord(&right, &D3DXVECTOR3(1, 0, 0), &R);
	
	position += velocity.z * forward * Time::Delta();
	position += velocity.x * right * Time::Delta();

	velocity.y += Gravity * Time::Delta();
	position.y += velocity.y * Time::Delta();

	if (position.y < terrainHeight)
	{
		position.y = terrainHeight;
		velocity.y = 0.0f;
		isJump = false;
	}

	CalcT();

	//초기화
	velocity.x = velocity.z = 0.0f;

	UpdateWorld(exValue);
}


void Transform_PhysiscC::Render(GlobalValues * exValue)
{
	SetWorld(exValue);
}

void Transform_PhysiscC::PostRender(GlobalValues * exValue)
{
}

void Transform_PhysiscC::Jump()
{
	if (!isJump)
	{
		isJump = true;
		velocity.y = JumpPower;
	}
}
