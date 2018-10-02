#pragma once
#include "Renders\IRenderer.h"

class MovingCubeBuffer : public ShaderBuffer
{
public:
	MovingCubeBuffer() : ShaderBuffer(&Data, sizeof(Struct))
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
class MovingCube : public IRenderer
{
public:
	MovingCube();
	virtual ~MovingCube();

	virtual void Init(ExecuteValues* exValue);
	virtual void Update(ExecuteValues* exValue);
	virtual void Render(ExecuteValues* exValue);
	virtual void PreRender(ExecuteValues* exValue);
	virtual void PostRender(ExecuteValues* exValue);
	virtual void ResizeScreen(ExecuteValues* exValue);

private:
	void CreateData();
	void CreateTexture();

private:
	typedef VertexTextureNormalTangentBinormal VertexType;

	MeshData<VertexType> *mesh;

	class IComponent* transform;

	MovingCubeBuffer* buffer;
	Shader* shader;

	vector<ID3D11ShaderResourceView *> views;
};