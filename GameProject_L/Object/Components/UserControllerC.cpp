#include "stdafx.h"
#include "UserControllerC.h"

#include "Transform_PhysiscC.h"
#include "../GameObject.h"

UserControllerC::UserControllerC(GameObject * object)
	:object(object), forward(0.0f,0.0f,1.0f), isJump(false)
{
	transformC = dynamic_cast<Transform_PhysiscC*>(object->GetComponent(ComponentFamilyID::TransformComponent));
}

UserControllerC::~UserControllerC()
{

}

void UserControllerC::Update(GlobalValues * exValue)
{
	if (transformC)
		Controll();
}

void UserControllerC::Render(GlobalValues * exValue)
{
}

void UserControllerC::PreRender(GlobalValues * exValue)
{
}

void UserControllerC::PostRender(GlobalValues * exValue)
{
}

void UserControllerC::Controll()
{
	//D3DXVECTOR3 velocity;
	//transformC->GetVelocity(velocity);
	D3DXVECTOR3& velocity = transformC->GetVelocity2();

	D3DXVECTOR3 rotation;
	transformC->GetRotation(rotation);

	if (Keyboard::Get()->Press(VK_LEFT))
		rotation.y -= TurnSpeed * Time::Delta();
	else if (Keyboard::Get()->Press(VK_RIGHT))
		rotation.y += TurnSpeed * Time::Delta();

	if (Keyboard::Get()->Press(VK_UP))
		velocity.z = MoveSpeed;
	else if (Keyboard::Get()->Press(VK_DOWN))
		velocity.z = -MoveSpeed;

	if (Keyboard::Get()->Press(VK_F1))
		velocity.x = -MoveSpeed;
	else if (Keyboard::Get()->Press(VK_F2))
		velocity.x = MoveSpeed;


	if (Keyboard::Get()->Press(VK_SPACE))
	{
		transformC->Jump();
	}

	//transformC->SetVelocity(velocity);
	transformC->SetRotation(rotation);
}
