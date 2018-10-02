#pragma once
#include "Renders/IRenderer.h"

class PMBuffer : public ShaderBuffer
{
public:
	PMBuffer() : ShaderBuffer(&Data, sizeof(Struct))
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

class PM : public IRenderer
{
public:
	PM(D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	virtual ~PM();

	virtual void Update(struct ExecuteValues* exValue);
	virtual void Render(ExecuteValues* exValue);
	virtual void PostRender(ExecuteValues* exValue);
	virtual void PreRender(ExecuteValues* exValue);

private:
	virtual void CreateData();
	void CreateTexture();

private:
	typedef VertexTextureNormalTangentBinormal VertexType;

	MeshData<VertexType> *mesh;

	D3DXVECTOR3 position;
	D3DXMATRIX world;

	PMBuffer* pBuffer;
	Shader* shader;

	vector<ID3D11ShaderResourceView *> views;
};