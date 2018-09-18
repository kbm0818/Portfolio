#include "stdafx.h"
#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
}

PerlinNoise::~PerlinNoise()
{
}

float PerlinNoise::noise(int x, int y, float scale)
{
	D3DXVECTOR2 pos(x * scale, y * scale);

	float x0 = floor(pos.x);
	float x1 = x0 + 1.0f;
	float y0 = floor(pos.y);
	float y1 = y0 + 1.0f;

	const D3DXVECTOR2& v0 = GetVec((int)x0, (int)y0);
	const D3DXVECTOR2& v1 = GetVec((int)x0, (int)y1);
	const D3DXVECTOR2& v2 = GetVec((int)x1, (int)y0);
	const D3DXVECTOR2& v3 = GetVec((int)x1, (int)y1);

	D3DXVECTOR2 d0(pos.x - x0, pos.y - y0);
	D3DXVECTOR2 d1(pos.x - x0, pos.y - y1);
	D3DXVECTOR2 d2(pos.x - x1, pos.y - y0);
	D3DXVECTOR2 d3(pos.x - x1, pos.y - y1);

	float h0 = D3DXVec2Dot(&d0, &v0);
	float h1 = D3DXVec2Dot(&d1, &v1);
	float h2 = D3DXVec2Dot(&d2, &v2);
	float h3 = D3DXVec2Dot(&d3, &v3);

	float Sx, Sy;

	//Perlin의 원래공식
	Sx = (3 * powf(d0.x, 2.0f)) - (2*powf(d0.x, 3.0f));
	Sy = (3 * powf(d0.y, 2.0f)) - (2*powf(d0.y, 3.0f));
	

	/*Sx = (6 * powf(d0.x, 5.0f)) - (15 * powf(d0.x, 4.0f)) + (10 * powf(d0.x, 3.0f));
	Sy = (6 * powf(d0.y, 5.0f)) - (15 * powf(d0.y, 4.0f)) + (10 * powf(d0.y, 3.0f));*/

	float avgX0 = h0 + (Sx * (h2 - h0));
	float avgX1 = h1 + (Sx * (h3 - h1));
	float result = avgX0 + (Sy * (avgX1 - avgX0));

	return result;
}

void PerlinNoise::Setup()
{
	float step = 6.24f / tableSize;
	float val = 0.0f;

	for (int i = 0; i < tableSize; i++)
	{
		vecTable[i].x = sin(val);
		vecTable[i].y = cos(val);
		val += step;

		lut[i] = rand() & tableMask;
	}
}

const D3DXVECTOR2 & PerlinNoise::GetVec(int x, int y) const
{
	unsigned char a = lut[x & tableMask];
	unsigned char b = lut[y & tableMask];
	unsigned char val = lut[(a + b) & tableMask];

	return vecTable[val];
}
