#pragma once
#include "Object\Shader\ShaderBuffer.h"

class FogBuffer : public ShaderBuffer
{
public:
	FogBuffer()
		: ShaderBuffer(&data, sizeof(Data))
	{
		data.fogColor = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
		data.zStart = 10.0f;
		data.zEnd = 50.0f;
		data.yStart = 3.0f;
		data.yEnd = 0.0f;
	}

	void SetZFog(float start, float end)
	{
		data.zStart = start;
		data.zEnd = end;
	}

	void SetYFog(float start, float end)
	{
		data.yStart = start;
		data.yEnd = end;
	}
	
private:
	friend class Fog;
	struct Data
	{
		D3DXCOLOR fogColor;
		float zStart;
		float zEnd;
		float yStart;
		float yEnd;
	};

	Data data;
};

class Fog
{
public:
	Fog();
	~Fog();

	void Render();
	void PostRender();

	void SetData(float zStart, float zEnd, float yStart, float yEnd);
	void SetZData(float zStart, float zEnd);
	void SetYData(float yStart, float yEnd);
	
private:
	FogBuffer* buffer;

};