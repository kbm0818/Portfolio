#pragma once

class SkyPlane_PerlinBuffer : public ShaderBuffer
{
public:
	SkyPlane_PerlinBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.translation = 0.0f;
		Data.scale = 0.0f;
		Data.brightness = 0.0f;
	}

	struct Struct
	{
		float translation;
		float scale;
		float brightness;
		float padding;
	};

	Struct Data;
};

class SkyPlane_Perlin
{
public:
	SkyPlane_Perlin();
	~SkyPlane_Perlin();

	void Update(Camera* camera);
	void Render(D3DXMATRIX* mat = nullptr);
	void PostRender(GuiSettings* settings);

private:
	bool Initialize(wstring, wstring);
	bool CreateVertexData(int, float, float, float, int);
	bool InitializeBuffers(int);
	void CreateState();
	bool LoadTextures(wstring, wstring);

private:
	struct SkyPlaneType
	{
		float x, y, z;
		float tu, tv;
	};

	typedef VertexTexture VertexType;

private:
	SkyPlaneType* skyPlane;
	int vertexCount, indexCount;
	ID3D11Buffer *vertexBuffer, *indexBuffer;

	float scale, brightness, translation;

	ID3D11ShaderResourceView *cloudTexture, *perturbTexture;
	ID3D11SamplerState *sampleState;

	SkyPlane_PerlinBuffer* skyPlaneBuffer;
	WorldBuffer* worldBuffer;
	Shader* shader;

	D3DXMATRIX world;
};