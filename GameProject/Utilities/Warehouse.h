#pragma once

class Warehouse
{
public:
	static float RandomNumberInRange(float Min, float Max)
	{
		float scalar = rand() * 2.3283064365386963e-10f;
		float range = Max - Min;

		return (Min + (scalar*range));
	}

	template <class T>
	static T clamp(T value, T low, T high)
	{
		if (value < low)
			return low;
		if (value > high)
			return high;
		return value;
	}

	//float absoluteValue(float val)
};