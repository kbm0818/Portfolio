#include "stdafx.h"
#include "Transform.h"

#include "../GameObject.h"

Transform::Transform() : ComponentBase(), parent(nullptr),
position(0,0,0), scale(1,1,1), up(0,1,0), right(1,0,0), forward(0,0,1)
{
	D3DXQuaternionIdentity(&rotation);
	D3DXMatrixIdentity(&matLocal);
	D3DXMatrixIdentity(&matWorld);
}

Transform::~Transform()
{
}

void Transform::Awake()
{
	UpdateAxis();
}

void Transform::Update()
{
	for (Transform* transform : childs)
	{
		if(transform->gameObject->IsActive())
			transform->gameObject->Update();
	}
}

void Transform::PreRender()
{
	for (Transform* transform : childs)
	{
		if (transform->gameObject->IsActive())
			transform->gameObject->PreRender();
	}
}

void Transform::Render()
{
	for (Transform* transform : childs)
	{
		if (transform->gameObject->IsActive())
			transform->gameObject->Render();
	}
}

void Transform::PostRender()
{
	for (Transform* transform : childs)
	{
		if (transform->gameObject->IsActive())
			transform->gameObject->PostRender();
	}
}

void Transform::SetPosition(D3DXVECTOR3 pos)
{
	position = pos;
	matLocal._41 = pos.x;
	matLocal._42 = pos.y;
	matLocal._43 = pos.z;
}

void Transform::SetRotation(D3DXQUATERNION rot)
{
	rotation = rot;
	if (rotation.w <= -0.099f)
		rotation = -rotation;

	UpdateAxis();
	UpdateMatrix();
}

void Transform::SetScale(D3DXVECTOR3 s)
{
	scale = s;
	UpdateMatrix();
}

D3DXVECTOR3 Transform::GetWorldPosition()
{
	if (!parent) return position;
	else
	{
		return position + parent->position;
	}
}

D3DXQUATERNION Transform::GetWorldRotation()
{
	if (!parent) return rotation;
	else
	{
		return rotation * parent->rotation;
	}
}

D3DXVECTOR3 Transform::GetWorldScale()
{
	if (!parent) return scale;
	else
	{
		D3DXVECTOR3 parentScale = parent->scale;
		return { scale.x*parentScale.x, scale.y*parentScale.y, scale.z*parentScale.z };
	}
}

D3DXMATRIX & Transform::GetWorldMatrix()
{
	if (parent)
		matWorld = parent->GetMatrix() * matLocal;
	else return matLocal;
	return matWorld;
}

void Transform::RemoveChild(Transform * t)
{
	for (size_t i = 0; i<childs.size(); ++i)
	{
		if (childs[i] == t)
		{
			Transform* transform = childs[i];
			childs.erase(childs.begin() + i);
			SAFE_DELETE(transform->gameObject);			
			return;
		}
	}
}

void Transform::UpdateMatrix()
{
	D3DXMATRIX t, r, s;

	D3DXMatrixRotationQuaternion(&r, &rotation);
	D3DXMatrixScaling(&s, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&t, position.x, position.y, position.z);

	matLocal = s * r * t;
}

void Transform::UpdateAxis()
{
	//D3DXQUATERNION worldRotation = GetWorldRotation();

	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &rotation);

	D3DXVec3TransformCoord(&right, &D3DXVECTOR3{ 1,0,0 }, &matRot);
	D3DXVec3TransformCoord(&up, &D3DXVECTOR3{ 0,1,0 }, &matRot);
	D3DXVec3TransformCoord(&forward, &D3DXVECTOR3{ 0,0,1 }, &matRot);
}
