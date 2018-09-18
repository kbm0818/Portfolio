#pragma once

class Frustum
{
public:
	Frustum();
	~Frustum();

	void ConstructFrustum(float screenDepth, D3DXMATRIX projectionMatrix, D3DXMATRIX viewMatrix);

	bool CheckPoint(float, float, float);
	bool CheckPoint(D3DXVECTOR3);

	bool CheckCube(float, float, float, float);

	bool CheckSphere(float, float, float, float);
	bool CheckSphere(D3DXVECTOR3, float);

	bool CheckRectangle(float, float, float, float, float, float);

private:
	D3DXPLANE m_planes[6];
};