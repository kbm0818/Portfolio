#pragma once

class BlurBuffer : public ShaderBuffer
{
public:
	BlurBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		data.height = 720.0f;
		data.width = 1028.0f;
	}
	
	void SetHeight(float height)
	{
		data.height = height;
	}

	void SetWidth(float width)
	{
		data.width = width;
	}

private:
	struct Data
	{
		float height;
		float width;
		D3DXVECTOR2 padding;
	};

	Data data;
};

class Blur
{
public:
	Blur(GlobalValues* values);
	~Blur();

	ID3D11ShaderResourceView* BlurTarget(wstring filePath);
	ID3D11ShaderResourceView* BlurTarget(ID3D11ShaderResourceView* targetView);

private:
	void Render();

	ID3D11ShaderResourceView* srvTarget;
	RenderTarget* renderTarget;
	Shader* blurShader;
	class D2DRender * d2dRender;
	BlurBuffer* buffer;
};