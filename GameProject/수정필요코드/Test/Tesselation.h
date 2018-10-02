#pragma once
#include "Renders/IRenderer.h"

class TesselationBuffer : public ShaderBuffer
{
public:
	TesselationBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.heightScale = 0.03f;
	}

	struct Struct
	{
		float heightScale;
		D3DXVECTOR3 padding;
	};

	Struct Data;
};

class Tesselation : public IRenderer
{
public:
	Tesselation(D3DXVECTOR3 position = D3DXVECTOR3(0.0f,0.0f,0.0f));
	virtual ~Tesselation();

	virtual void Update(struct ExecuteValues* exValue);
	virtual void Render(ExecuteValues* exValue);
	virtual void PostRender(ExecuteValues* exValue);
	virtual void PreRender(ExecuteValues* exValue);

private:
	void CreateData();
	void CreateTexture();
	
	typedef VertexTextureNormalTangentBinormal VertexType;

	MeshData<VertexType> *mesh;

	D3DXVECTOR3 position;
	D3DXMATRIX world;

	TesselationBuffer* pBuffer;
	Shader* shader;

	vector<ID3D11ShaderResourceView *> views;
};