#include "stdafx.h"
#include "Camera.h"

Camera::Camera()
	: position(0, 0, 0), rotation(0, 0)
	, forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
{
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matView);
	D3DXMatrixIdentity(&reflectionView);

	UpdateRotation();
	UpdateView();
}

Camera::~Camera()
{

}

void Camera::GetMatrix(D3DXMATRIX * mat)
{
	memcpy(mat, &matView, sizeof(D3DXMATRIX));
}

void Camera::GetReflectionMatrix(D3DXMATRIX * mat)
{
	memcpy(mat, &reflectionView, sizeof(D3DXMATRIX));
}

void Camera::UpdateMove()
{
	UpdateView();
}

void Camera::UpdateRotation()
{
	D3DXMATRIX x, y;
	D3DXMatrixRotationX(&x, rotation.x);
	D3DXMatrixRotationY(&y, rotation.y);

	matRotation = x * y;

	D3DXVec3TransformCoord(&forward, &D3DXVECTOR3(0, 0, 1), &matRotation);
	D3DXVec3TransformCoord(&right, &D3DXVECTOR3(1, 0, 0), &matRotation);
	D3DXVec3TransformCoord(&up, &D3DXVECTOR3(0, 1, 0), &matRotation);
}

void Camera::UpdateView()
{
	D3DXMatrixLookAtLH(&matView, &position, &(position + forward), &up);
}

void Camera::UpdateReflectionView(float height)
{
	D3DXVECTOR3 r_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXVECTOR3 r_position = D3DXVECTOR3(position.x, -position.y + (height * 2.0f), position.z);
	D3DXVECTOR3 r_lookAt = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMATRIX x, y;
	D3DXMatrixRotationX(&x, -rotation.x);
	D3DXMatrixRotationY(&y, rotation.y);

	reflectionView = x * y;

	D3DXVec3TransformCoord(&r_lookAt, &r_lookAt, &reflectionView);
	D3DXVec3TransformCoord(&r_up, &r_up, &reflectionView);

	r_lookAt = r_position + r_lookAt;

	D3DXMatrixLookAtLH(&reflectionView, &r_position, &r_lookAt, &r_up);
}
