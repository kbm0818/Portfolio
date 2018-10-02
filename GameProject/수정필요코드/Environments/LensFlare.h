#pragma once

class LensFlareBuffer : public ShaderBuffer
{
public:
	LensFlareBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.flareColor = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f);
		Data.posOffSet = D3DXVECTOR2(0.0f, 0.0f);
		Data.uvOffSet = D3DXVECTOR2(0.0f, 0.0f);
		Data.scale = 0.0f;
	}

	struct Struct
	{
		D3DXCOLOR flareColor;
		D3DXVECTOR2 posOffSet;
		D3DXVECTOR2 uvOffSet;
		float scale;
		D3DXVECTOR3 padding;
	};

	Struct Data;
};

class LensFlare
{
public:
	LensFlare();
	~LensFlare();

	void Update(D3DXVECTOR3 camPos, D3DXVECTOR3 sunDir, D3DXMATRIX view, D3DXMATRIX projection);
	void Render();
	void PostRender(GuiSettings* settings);

	enum e_constants
	{
		k_totalFlares = 9,
	};

private:
	bool Initialize(wstring);
	bool InitializeBuffers();
	void CreateState();
	bool LoadTextures(wstring);

	struct sFlareElement
	{
		float linePos;		// position on line (-1 to 1)
		float scale;		// screen-based scale
		float brightness;	// emissive property (0-1)
		float texU;		// x location in texture
		float texV;		// y location in texture
	};

private:
	typedef VertexTexture_2D VertexType;

	UINT vertexCount, indexCount;
	ID3D11Buffer *vertexBuffer, *indexBuffer;

	ID3D11ShaderResourceView *lensFlareTexture;
	ID3D11SamplerState *sampleState;
	ID3D11BlendState *alphaBlendEnableState, *alphaBlendDisableState;
	ID3D11DepthStencilState *depthOnState, *depthOffState;

	LensFlareBuffer* lensFlareBuffer;

	Shader* shader;

	sFlareElement m_flareElements[k_totalFlares];

	bool isVisible;

	D3DXVECTOR3 screenPos;
};