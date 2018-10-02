#include "stdafx.h"
#include "TransformC.h"
#include "Renderers/Renderers.h"

TransformC::TransformC()
	: position(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, rotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, scale(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, center(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
	, isMove(false), slot(1)
{
	D3DXMatrixIdentity(&S);
	D3DXMatrixIdentity(&R);
	D3DXMatrixIdentity(&T);
	D3DXMatrixIdentity(&world);

	D3DXQuaternionIdentity(&rotate);
	buffer = new WorldBuffer();
}

TransformC::~TransformC()
{
	SAFE_DELETE(buffer);
}

void TransformC::Update(GlobalValues * exValue)
{
	
}

void TransformC::PreRender(GlobalValues * exValue)
{
	//이동된 값에 의한 worldMat 수정
	if (isMove)
	{
		UpdateWorld(exValue);
	}
}


void TransformC::Render(GlobalValues * exValue)
{
	SetWorld(exValue);
}


void TransformC::PostRender(GlobalValues * exValue)
{
}

void TransformC::SetPosition(const D3DXVECTOR3 & vec)
{
	this->position = vec;
	CalcT();
	isMove = true;
}

void TransformC::SetScale(const D3DXVECTOR3 & vec)
{
	this->scale = vec;
	CalcS();
	isMove = true;
}

void TransformC::SetScale(const float & s)
{
	this->scale = D3DXVECTOR3(s, s, s);
	CalcS();
	isMove = true;
}

void TransformC::SetRotation(const D3DXVECTOR3 & vec)
{
	this->rotation = vec;
	CalcR();
	isMove = true;
}

void TransformC::CalcS()
{
	D3DXMatrixScaling(&S, scale.x, scale.y, scale.z);
}

void TransformC::CalcR()
{
	D3DXQuaternionRotationYawPitchRoll(&rotate, rotation.y, rotation.x, rotation.z);
	D3DXMatrixRotationQuaternion(&R, &rotate);
}

void TransformC::CalcT()
{
	D3DXMatrixTranslation(&T, position.x, position.y, position.z);
}

void TransformC::UpdateWorld(GlobalValues* exValue)
{
	world = S * R * T;

	buffer->SetMatrix(world);
	isMove = false;
}

void TransformC::SetWorld(GlobalValues* exValue)
{
	buffer->SetVSBuffer(slot);
}
