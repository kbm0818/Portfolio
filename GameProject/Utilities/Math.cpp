#include "stdafx.h"
#include "Math.h"

const float Math::PI = 3.14159265f;

float Math::ToRadian(float degree)
{
	return degree * PI / 180.0f;
}

float Math::ToDegree(float radian)
{
	return radian * 180.0f / PI;
}

int Math::RandomRange(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}