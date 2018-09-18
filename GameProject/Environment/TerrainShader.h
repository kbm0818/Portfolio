#pragma once

class TerrainBuffer : public ShaderBuffer
{
public:
	TerrainBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.Apex = D3DXCOLOR(0.0f, 0.05f, 0.6f, 1.0f);
		Data.Center = D3DXCOLOR(0.0f, 0.5f, 0.8f, 1.0f);
	}

	struct Struct
	{
		D3DXCOLOR Center;
		D3DXCOLOR Apex;
	};

	Struct Data;
};


class TerrainShader : public Shader
{
public:
	TerrainShader(wstring shaderFile);
	virtual ~TerrainShader();

	virtual void Render();
	virtual void PostRender();
	virtual void SetStates();

private:
	TerrainBuffer * terrainBuffer;
};