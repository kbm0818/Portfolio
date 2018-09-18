#pragma once
#include "stdafx.h"

class QuaternionUtility
{
public:
	//now: 현재 Rotation, destDegree: 목표 각도, stepAngle: 스피드(degree)
	static D3DXQUATERNION RotateTowards(D3DXQUATERNION now, D3DXVECTOR3 destDegree, float stepAngle)
	{
		int sign = 1;
		D3DXQUATERNION dest, step, direction;
		stepAngle = (float)D3DXToRadian(stepAngle);

		D3DXQuaternionRotationYawPitchRoll(&dest,
			(float)D3DXToRadian(destDegree.y),
			(float)D3DXToRadian(destDegree.x),
			(float)D3DXToRadian(destDegree.z));

		direction = dest - now;
		if (D3DXQuaternionLengthSq(&direction) > D3DXQuaternionLengthSq(&(dest + now)))
		{
			dest = -dest;
			sign = -1;
		}

		D3DXVECTOR3 axis;
		float temp;
		D3DXQuaternionToAxisAngle(&direction, &axis, &temp);
		D3DXQuaternionRotationAxis(&step, &axis, sign*stepAngle);

		//거리 비교
		D3DXQUATERNION q = now * step;
		if (D3DXQuaternionLengthSq(&(dest - now)) > D3DXQuaternionLengthSq(&(q - now)))
		{
			return q;
		}
		else
		{
			return dest;
		}
	}
private:

};