#pragma once
#include "Renders/IRenderer.h"

class POMBuffer : public ShaderBuffer
{
public:
	POMBuffer() : ShaderBuffer(&Data, sizeof(Struct))
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

struct ExecuteValues;
class POM : public IRenderer
{
public:
	POM(D3DXVECTOR3 position = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	virtual ~POM();

	virtual void Update(ExecuteValues* exValue);
	virtual void Render(ExecuteValues* exValue);
	virtual void PostRender(ExecuteValues* exValue);
	virtual void PreRender(ExecuteValues* exValue);

private:
	void CreateData();
	void CreateTexture();

private:
	typedef VertexTextureNormalTangentBinormal VertexType;

	MeshData<VertexType> *mesh;

	D3DXVECTOR3 position;
	D3DXMATRIX world;

	POMBuffer* pBuffer;
	Shader* shader;

	vector<ID3D11ShaderResourceView *> views;
};