#pragma once
#include "../Collider.h"

class Circle;
class Capsule2D;
class CapsuleCollider : public Collider
{
public:
	CapsuleCollider(float radius, float height);
	virtual ~CapsuleCollider();
	CapsuleCollider(CapsuleCollider& copy);

	virtual void Render();
	virtual bool IsCollision(Collider* other);

	void UpdateVertex(float radius, float height);
	float GetRadius() { return radius; }
	float GetHeight() { return height; }

private:
	typedef VertexColor VertexType;

	bool IsCollisionCapsuleCapsule(CapsuleCollider* other);
	float ProjectionToLineAbs(D3DXVECTOR3 vec, D3DXVECTOR3 normal);
	float PointToLineDistance(D3DXVECTOR3 lookAt, D3DXVECTOR3 normal);

	float height;
	float radius;

	Circle* circle[2];
	Capsule2D* capsule2D[2];

	D3DXVECTOR3 GetAPos();
	D3DXVECTOR3 GetBPos();
	D3DXVECTOR3 GetLocalAPos();
	D3DXVECTOR3 GetLocalBPos();

	bool IsCollisionSphere(D3DXVECTOR3 a, D3DXVECTOR3 b, float sumRad);
	bool IsCollision1D(float pos, float targetPos, float radius, float targetRadius);

	D3DXVECTOR3 Get3DRadius();
	D3DXVECTOR3 AbsVector3(D3DXVECTOR3 v);
};