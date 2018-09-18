#include "stdafx.h"
#include "LookAtCamera.h"
//#include "Model\ModelScene.h"

LookAtCamera::LookAtCamera(float moveSpeed, float rotationSpeed)
	: moveSpeed(moveSpeed), rotationSpeed(rotationSpeed),
	distance(10.0f), height(1.0f), angle(-90.0f)//, target(nullptr)
{
	UpdatePosition();
}

LookAtCamera::~LookAtCamera()
{

}

void LookAtCamera::Update()
{
	if (Keyboard::Get()->Press('W'))
	{
		distance -= moveSpeed * Time::Delta();
		UpdatePosition();
	}
	else if (Keyboard::Get()->Press('S'))
	{
		distance += moveSpeed * Time::Delta();
		UpdatePosition();
	}

	if (Keyboard::Get()->Press('A'))
	{
		angle -= rotationSpeed * Time::Delta();
		UpdatePosition();
	}
	else if (Keyboard::Get()->Press('D'))
	{
		angle += rotationSpeed * Time::Delta();
		UpdatePosition();
	}

	if (Keyboard::Get()->Press('Q'))
	{
		height += moveSpeed * Time::Delta();
		UpdatePosition();
	}
	else if (Keyboard::Get()->Press('E'))
	{
		height -= moveSpeed * Time::Delta();
		UpdatePosition();
	}
}
//
//void LookAtCamera::SetTarget(ModelScene * target)
//{
//	this->target = target;
//	UpdatePosition();
//}

void LookAtCamera::UpdateView()
{
	//if (target == nullptr)
		D3DXMatrixLookAtLH(&matView, &position, &D3DXVECTOR3{ 0, 2, 0 }, &up);
	//else
	{
		//D3DXMatrixLookAtLH(&matView, &position, &(target->GetCenter()), &up);
	}
}

void LookAtCamera::UpdatePosition()
{
	D3DXVECTOR3 center = { 0, 5, 0 };
	//if (target)
		//center = target->GetCenter();

	position.x = cosf((float)D3DXToRadian(angle)) * distance;
	position.y = height;
	position.z = sinf((float)D3DXToRadian(angle)) * distance;

	UpdateView();
}
