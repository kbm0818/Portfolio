#pragma once

class SkyPlaneBuffer : public ShaderBuffer
{
public:
	SkyPlaneBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.firstTranslationX = 0.0f;
		Data.firstTranslationZ = 0.0f;
		Data.secondTranslationX = 0.0f;
		Data.secondTranslationZ = 0.0f;
		Data.brightness = 0.0f;
	}

	struct Struct
	{
		float firstTranslationX;
		float firstTranslationZ;
		float secondTranslationX;
		float secondTranslationZ;
		float brightness;
		D3DXVECTOR3 padding;
	};

	Struct Data;
};

class SkyPlane
{
public:
	SkyPlane();
	~SkyPlane();

	void Update();
	void Render();
	void PostRender(GuiSettings* settings);

private:
	bool Initialize(wstring, wstring);
	bool CreateData(int, float, float, float, int);
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
	
	float m_brightness;
	float m_translationSpeed[4];
	float m_textureTranslation[4];

	ID3D11ShaderResourceView *cloudTexture1, *cloudTexture2;
	ID3D11SamplerState *sampleState;
	ID3D11BlendState *alphaBlendEnableState, *alphaBlendDisableState;

	SkyPlaneBuffer* skyPlaneBuffer;
	Shader* shader;

	WorldBuffer* worldBuffer;

	D3DXMATRIX world;
};