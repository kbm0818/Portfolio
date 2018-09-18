#include "stdafx.h"
#include "Collider.h"
#include "GameCore\GameObject\IEventArgs.h"

Collider::Collider(Type type):forward(0, 0, 1), up(0, 1, 0), right(1, 0, 0),
position(0, 0, 0), type(type), parentPosition(0,0,0), gameObject(nullptr)
{
	name = L"collider";

	state = None;
	collisionCount = 0;

	D3DXQuaternionIdentity(&rotation);
	D3DXQuaternionIdentity(&worldRotation);
	D3DXQuaternionIdentity(&parentRotation);
	D3DXQuaternionIdentity(&prevRotation);

	UpdateWorldPosition();
	UpdateWorldRotation();
}

Collider::~Collider()
{
}

void Collider::CheckExit()
{
	if (state == Stay)
	{
		collisionCount--;
		if (collisionCount <= 0)
		{
			gameObject->SendEvent("OnCollisionExit", CollisionEventArgs(nullptr));
			state = None;
		}
	}
}

void Collider::Render()
{
}

void Collider::Collision(Collider * other)
{
	if (!gameObject)
	{
		OutputDebugStringA("Collider is No GameObject");
		return;
	}

	collisionCount++;
	switch (state)
	{
	case Collider::None:
		state = Enter;
		gameObject->SendEvent("OnCollisionEnter", CollisionEventArgs(other));
		break;
	case Collider::Enter:
		state = Stay;
		gameObject->SendEvent("OnCollisionStay", CollisionEventArgs(other));
		break;
	case Collider::Stay:
		gameObject->SendEvent("OnCollisionStay", CollisionEventArgs(other));
		break;
	}

}

string Collider::GetStringName()
{
	return String::WStringToString(name);
}

string Collider::GetTypeName()
{
	string typeName = "None";

	switch (type)
	{
	case Collider::CAPSULE:
		typeName = "Capsule";
		break;
	}

	return typeName;
}

void Collider::UpdateAxis()
{
	if (prevRotation != worldRotation)
	{
		D3DXMATRIX matRot;
		D3DXMatrixRotationQuaternion(&matRot, &worldRotation);

		D3DXVec3TransformCoord(&right, &D3DXVECTOR3{ 1,0,0 }, &matRot);
		D3DXVec3TransformCoord(&up, &D3DXVECTOR3{ 0,1,0 }, &matRot);
		D3DXVec3TransformCoord(&forward, &D3DXVECTOR3{ 0,0,1 }, &matRot);

		prevRotation = worldRotation;
	}
}

D3DXVECTOR3 Collider::GetLocalForward()
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &rotation);

	D3DXVECTOR3 result;
	D3DXVec3TransformCoord(&result, &D3DXVECTOR3{ 0,0,1 }, &matRot);

	return result;
}

D3DXVECTOR3 Collider::GetLocalUp()
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &rotation);

	D3DXVECTOR3 result;
	D3DXVec3TransformCoord(&result, &D3DXVECTOR3{ 0,1,0 }, &matRot);

	return result;
}

D3DXVECTOR3 Collider::GetLocalRight()
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &rotation);

	D3DXVECTOR3 result;
	D3DXVec3TransformCoord(&result, &D3DXVECTOR3{ 1,0,0 }, &matRot);

	return result;
}
