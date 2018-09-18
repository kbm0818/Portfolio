#pragma once

class Font
{
public:
	const UINT MaxSize = 80;

private:
	struct CharsetDesc
	{
		UINT X = 0, Y = 0;
		UINT Width = 0, Height = 0;
		
		float OffsetX = 0.0f, OffsetY = 0.0f;
		float AdvanceX = 0.0f;

		UINT Page = 0;
	};

	struct FontDesc
	{
		UINT LineHeight = 0;
		UINT Base = 0;
		UINT Width = 0, Height = 0;
		UINT PageCount = 0;

		CharsetDesc desc[256];
	};

public:
	Font(struct ExecuteValues* values, wstring file, wstring texture);
	~Font();

	void Update();
	void Render();

private:
	void CreateBuffer();
	void Parse(wstring file, wstring texture);

private:
	struct ExecuteValues* values;

	Shader* shader;
	WorldBuffer* worldBuffer;

	D3DXMATRIX view;
	class Orthographic* ortho;

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	ID3D11ShaderResourceView* srv;

	VertexTexture* vertices;
	UINT* indices;

	string test;
	D3DXVECTOR2 testLocation;

	FontDesc fontDesc;
	bool bRemap;

	ID3D11BlendState* blendState[2];
	ID3D11DepthStencilState* depthState[2];
};