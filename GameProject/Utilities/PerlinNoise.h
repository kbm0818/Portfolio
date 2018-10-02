#pragma once

class PerlinNoise
{
public:
	enum
	{
		tableSize = 256,
		tableMask = tableSize - 1,
	};

	PerlinNoise();
	~PerlinNoise();

	float noise(int x, int y, float scale);
	void Setup();

private:
	const D3DXVECTOR2& GetVec(int x, int y) const;

private:
	D3DXVECTOR2 vecTable[tableSize];
	unsigned char lut[tableSize];

};