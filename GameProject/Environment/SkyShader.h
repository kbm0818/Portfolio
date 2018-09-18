#pragma once

class SkyBuffer : public ShaderBuffer
{
public:
	SkyBuffer() : ShaderBuffer(&Data, sizeof(Struct))
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


class SkyShader : public Shader
{
public:
	SkyShader(wstring shaderFile);
	virtual ~SkyShader();

	virtual void Render();
	virtual void PostRender();
	virtual void SetStates();

private:
	SkyBuffer * skyBuffer;
};