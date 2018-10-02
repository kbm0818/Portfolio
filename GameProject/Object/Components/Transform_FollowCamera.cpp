#include "stdafx.h"
#include "Transform_FollowCamera.h"

Transform_FollowCamera::Transform_FollowCamera()
{
}

Transform_FollowCamera::~Transform_FollowCamera()
{
}

void Transform_FollowCamera::Update(GlobalValues * exValue)
{
}

void Transform_FollowCamera::PreRender(GlobalValues * exValue)
{
	exValue->MainCamera->GetPosition(&position);
	CalcT();

	UpdateWorld(exValue);
}

void Transform_FollowCamera::Render(GlobalValues * exValue)
{
	SetWorld(exValue);
}

void Transform_FollowCamera::PostRender(GlobalValues * exValue)
{
}
