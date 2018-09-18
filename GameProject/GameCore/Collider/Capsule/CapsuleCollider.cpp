#include "stdafx.h"
#include "CapsuleCollider.h"
#include "../../2DObject/Circle.h"
#include "../../2DObject/Capsule2D.h"

CapsuleCollider::CapsuleCollider(float radius, float height) :
	radius(radius), height(height), Collider(Type::CAPSULE)
{
	for (int i = 0; i < 2; ++i)
	{
		circle[i] = new Circle(D3DXVECTOR3(0,0,0), { 0,0,0 });
		capsule2D[i] = new Capsule2D(D3DXVECTOR3(0, 0, 0), { 0,0,0 });
	}
	UpdateVertex(radius, height);
}

CapsuleCollider::~CapsuleCollider()
{
	for (int i = 0; i < 2; ++i)
	{
		SAFE_DELETE(circle[i]);
		SAFE_DELETE(capsule2D[i]);
	}
}

CapsuleCollider::CapsuleCollider(CapsuleCollider & copy)
	: Collider(Type::CAPSULE)
{
	name = copy.name;
	height = copy.height;
	radius = copy.radius;
	for (int i = 0; i < 2; ++i)
	{
		circle[i] = new Circle(D3DXVECTOR3(0, 0, 0), { 0,0,0 });
		capsule2D[i] = new Capsule2D(D3DXVECTOR3(0, 0, 0), { 0,0,0 });
	}
	position = copy.position;
	rotation = copy.rotation;
	UpdateVertex(radius, height);
}

void CapsuleCollider::Render()
{
	D3DXVECTOR3 A = GetLocalAPos(), B = GetLocalBPos();

	//회전 행렬 만들기.
	D3DXQUATERNION rotateY, rotateX;
	D3DXQuaternionRotationYawPitchRoll(&rotateY, (float)D3DXToRadian(90), 0, 0);
	D3DXQuaternionRotationYawPitchRoll(&rotateX, 0, (float)D3DXToRadian(90), 0);

	D3DXMATRIX rX, rY, r;
	D3DXMatrixRotationQuaternion(&rX, &(rotateX * rotation));
	D3DXMatrixRotationQuaternion(&rY, &(rotateY * rotation));
	D3DXMatrixRotationQuaternion(&r, &rotation);

	//이동 행렬 만들기.
	D3DXMATRIX t, tA, tB;
	D3DXMatrixTranslation(&t, position.x, position.y, position.z);
	D3DXMatrixTranslation(&tA, A.x, A.y, A.z);
	D3DXMatrixTranslation(&tB, B.x, B.y, B.z);

	capsule2D[0]->SetWorld((rY * t) * matParent);
	capsule2D[1]->SetWorld((r * t) * matParent);
	circle[0]->SetWorld((rX * tA) * matParent);
	circle[1]->SetWorld((rX * tB) * matParent);
	

	for (int i = 0; i < 2; ++i)
	{
		circle[i]->Render();
		capsule2D[i]->Render();
	}
}

bool CapsuleCollider::IsCollision(Collider* other)
{
	UpdateAxis();
	if (other->GetType() == Type::CAPSULE)
	{
		return IsCollisionCapsuleCapsule(dynamic_cast<CapsuleCollider*>(other));
	}
	return false;
}

void CapsuleCollider::UpdateVertex(float radius, float height)
{
	for (int i = 0; i < 2; ++i)
	{
		circle[i]->UpdateVertex(radius);
		capsule2D[i]->UpdateVertex(radius, height);
	}
	this->radius = radius;
	this->height = height;
}

D3DXVECTOR3 CapsuleCollider::GetAPos()
{
	D3DXVECTOR3 A = GetLocalAPos();
	D3DXVec3TransformCoord(&A, &A, &matParent);
	return A;
}

D3DXVECTOR3 CapsuleCollider::GetBPos()
{
	D3DXVECTOR3 B = GetLocalBPos();
	D3DXVec3TransformCoord(&B, &B, &matParent);
	return B;
}

D3DXVECTOR3 CapsuleCollider::GetLocalAPos()
{
	return position + GetLocalUp() * height * 0.5f;
}

D3DXVECTOR3 CapsuleCollider::GetLocalBPos()
{
	UpdateAxis();
	return position - GetLocalUp() * height * 0.5f;
}

bool CapsuleCollider::IsCollisionSphere(D3DXVECTOR3 a, D3DXVECTOR3 b, float sumRad)
{
	float length = D3DXVec3Length(&(a - b));
	if (length < sumRad)
		return true;
	return false;
}

bool CapsuleCollider::IsCollision1D(float pos, float targetPos, float radius, float targetRadius)
{
	float length = abs(pos - targetPos);
	if (length < radius + targetRadius)
		return true;
	return false;
}

D3DXVECTOR3 CapsuleCollider::Get3DRadius()
{
	D3DXVECTOR3 rad = { radius, height * 0.5f + radius, radius };

	float yRadius = D3DXVec3Dot(&rad, &AbsVector3(up));
	float zRadius = D3DXVec3Dot(&rad, &AbsVector3(forward));
	float xRadius = D3DXVec3Dot(&rad, &AbsVector3(right));

	return { xRadius, yRadius, zRadius };
}

D3DXVECTOR3 CapsuleCollider::AbsVector3(D3DXVECTOR3 v)
{
	v.x = abs(v.x);
	v.y = abs(v.y);
	v.z = abs(v.z);
	return v;
}




bool CapsuleCollider::IsCollisionCapsuleCapsule(CapsuleCollider * other)
{
	if (!other) return false;

	float _radius1 = radius;
	float _height1 = height;
	float _radius2 = other->GetRadius();
	float _height2 = other->GetHeight();

	D3DXVECTOR3 dir1 = this->GetUp();
	D3DXVECTOR3 dir2 = other->GetUp();

	D3DXVECTOR3 center1 = this->GetWorldPosition();
	D3DXVECTOR3 center2 = other->GetWorldPosition();

	D3DXVECTOR3 w0 = center2 - center1;

	D3DXVECTOR3 endPoint11 = this->GetAPos();
	D3DXVECTOR3 endPoint12 = this->GetBPos();
	D3DXVECTOR3 endPoint21 = other->GetAPos();
	D3DXVECTOR3 endPoint22 = other->GetBPos();

	float a = D3DXVec3Dot(&dir1, &dir1);
	float b = D3DXVec3Dot(&dir1, &dir2);
	float c = D3DXVec3Dot(&dir2, &dir2);
	float d = D3DXVec3Dot(&dir1, &w0);
	float e = D3DXVec3Dot(&dir2, &w0);

	float distance;

	//==================일단 끝점쪽이 붙으면 빠른 연산을 위해 미리 충돌처리
	distance = D3DXVec3Length(&(endPoint11 - endPoint21));
	if (distance < _radius1 + _radius2)
	{
		return true;
	}
	distance = D3DXVec3Length(&(endPoint12 - endPoint22));
	if (distance < _radius1 + _radius2)
	{
		return true;
	}
	distance = D3DXVec3Length(&(endPoint11 - endPoint22));
	if (distance < _radius1 + _radius2)
	{
		return true;
	}
	distance = D3DXVec3Length(&(endPoint12 - endPoint21));
	if (distance < _radius1 + _radius2)
	{
		return true;
	}
	//=======================================================


	if (a * c - b * b == 0)
	{
		//서로 평행할때

		D3DXVECTOR3 centerToEndPoint;
		float len;

		centerToEndPoint = endPoint22 - center1;
		len = ProjectionToLineAbs(centerToEndPoint, dir1);
		if (len < _height1 / 2)
		{
			distance = PointToLineDistance(centerToEndPoint, dir1);

			if (distance < _radius1 + _radius2)
				return true;
		}

		centerToEndPoint = endPoint21 - center1;
		len = ProjectionToLineAbs(centerToEndPoint, dir1);
		if (len < _height1 / 2)
		{
			distance = PointToLineDistance(centerToEndPoint, dir1);

			if (distance < _radius1 + _radius2)
				return true;
		}

		centerToEndPoint = endPoint12 - center2;
		len = ProjectionToLineAbs(centerToEndPoint, dir2);
		if (len < _height2 / 2)
		{
			distance = PointToLineDistance(centerToEndPoint, dir2);

			if (distance < _radius1 + _radius2)
				return true;
		}

		centerToEndPoint = endPoint11 - center2;
		len = ProjectionToLineAbs(centerToEndPoint, dir2);
		if (len < _height2 / 2)
		{
			distance = PointToLineDistance(centerToEndPoint, dir2);

			if (distance < _radius1 + _radius2)
				return true;
		}
	}

	if (a * c - b * b != 0)
	{
		//평행이 아닐경우 즉 교점이 있거나 꼬인위치이다
		D3DXVECTOR3 p1;
		D3DXVECTOR3 p2;
		float t1;
		float t2;
		bool bCap1In = false;
		bool bCap2In = false;

		t1 = (b * e - c * d) / (a * c - b * b);
		t2 = (a * e - b * d) / (a * c - b * b);

		p1 = center1 + dir1 * t1;      //최소거리리 이루는 점의 위치
		p2 = center2 + dir2 * t2;

		if (t1 >= -_height1 / 2 && t1 <= _height1 / 2) bCap1In = true;
		if (t2 >= -_height2 / 2 && t2 <= _height2 / 2) bCap2In = true;

		if (!bCap1In && !bCap2In)
		{
			if (t1 * t1 > t2 * t2)  // 2번 캡슐의 직선상위에 교점이 존재할 경우
			{
				//튀어나온쪽으로의 벡터
				D3DXVECTOR3 end1 = center1 + dir1 * t1 / fabsf(t1) * _height1 / 2;
				float len = D3DXVec3Dot(&(end1 - center2), &dir2);

				if (len > _height2 / 2) len = _height2 / 2;
				else if (len < -_height2 / 2) len = -_height2 / 2;

				D3DXVECTOR3 temp = center2 + dir2 * len;
				distance = D3DXVec3Length(&(temp - end1));
				if (distance < _radius1 + _radius2)
				{
					return true;
				}
			}
			else if (t1 * t1 < t2 * t2) // 1번 캡슐의 직선상위에 교점이 존재할 경우
			{
				D3DXVECTOR3 end2 = center2 + dir2 * t2 / fabsf(t2) * _height2 / 2;
				float len = D3DXVec3Dot(&(end2 - center1), &dir1);

				if (len > _height1 / 2) len = _height1 / 2;
				else if (len < -_height1 / 2) len = -_height1 / 2;

				D3DXVECTOR3 temp = center1 + dir1 * len;
				distance = D3DXVec3Length(&(temp - end2));
				if (distance < _radius1 + _radius2)
				{
					return true;
				}

			}
		}

		else if (!bCap1In && bCap2In)
		{
			//cap1의 끝점과 cap2의 직선의 거리를 구해서 체크한다
			D3DXVECTOR3 centerToEndPoint;

			centerToEndPoint = endPoint11 - center2;
			distance = PointToLineDistance(centerToEndPoint, dir2);
			if (distance < _radius1 + _radius2)
			{
				return true;
			}

			centerToEndPoint = endPoint12 - center2;
			distance = PointToLineDistance(centerToEndPoint, dir2);
			if (distance < _radius1 + _radius2)
			{
				return true;
			}
		}

		else if (bCap1In && !bCap2In)
		{
			//cap2의 끝점과 cap1의 직선의 거리를 구해서 체크한다
			D3DXVECTOR3 centerToEndPoint;

			centerToEndPoint = endPoint21 - center1;
			distance = PointToLineDistance(centerToEndPoint, dir1);
			if (distance < _radius1 + _radius2)
			{
				return true;
			}

			centerToEndPoint = endPoint22 - center1;
			distance = PointToLineDistance(centerToEndPoint, dir1);
			if (distance < _radius1 + _radius2)
			{
				return true;
			}
		}

		else if (bCap1In && bCap2In)
		{
			//최단점이 두선분 안에 있을경우다
			distance = D3DXVec3Length(&(p1 - p2));
			if (distance < _radius1 + _radius2)
			{
				return true;
			}
		}
	}
	return false;










	////반지름 구하기
	//D3DXVECTOR3 R = Get3DRadius();
	//D3DXVECTOR3 targetR = other->Get3DRadius();

	////충돌 체크
	//// - X
	//if (!IsCollision1D(worldPosition.x, other->worldPosition.x,
	//	R.x, targetR.x)) return false;

	//// - Y
	//if (!IsCollision1D(worldPosition.y, other->worldPosition.y,
	//	R.y, targetR.y)) return false;

	//// - Z
	//if (!IsCollision1D(worldPosition.z, other->worldPosition.z,
	//	R.z, targetR.z)) return false;

	////위의 러프 체크를 통과했을 경우 캡슐의 각 끝쪽 구끼리 충돌체크를해서 대각선 체크를 한다.
	//D3DXVECTOR3 A = GetAPos();
	//D3DXVECTOR3 targetA = other->GetAPos();
	//float sumRadius = radius + other->radius;

	//if (IsCollisionSphere(A, targetA, sumRadius))
	//	return true;

	//D3DXVECTOR3 B = GetBPos();
	//D3DXVECTOR3 targetB = other->GetBPos();
	//if (IsCollisionSphere(B, targetB, sumRadius))
	//	return true;

	//if (IsCollisionSphere(B, targetA, sumRadius))
	//	return true;

	//if (IsCollisionSphere(A, targetB, sumRadius))
	//	return true;

	//return false;
}

float CapsuleCollider::ProjectionToLineAbs(D3DXVECTOR3 vec, D3DXVECTOR3 normal)
{
	return fabsf(D3DXVec3Dot(&vec, &normal));
}

float CapsuleCollider::PointToLineDistance(D3DXVECTOR3 lookAt, D3DXVECTOR3 normal)
{
	D3DXVECTOR3 lookAtProj;
	D3DXVec3Cross(&lookAtProj, &lookAt, &normal);
	return D3DXVec3Length(&lookAtProj) / D3DXVec3Length(&normal);
}