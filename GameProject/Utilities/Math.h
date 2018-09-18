#pragma once
class Math
{
public:
	static const float PI;

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static int RandomRange(int r1, int r2);
};