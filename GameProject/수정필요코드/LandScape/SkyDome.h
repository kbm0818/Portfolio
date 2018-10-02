#pragma once
#include "Renders/IRenderer.h"

class SkyDomeBuffer : public ShaderBuffer
{
public:
	SkyDomeBuffer() : ShaderBuffer(&Data, sizeof(Struct))
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

class SkyDome : public IRenderer
{
public:
	SkyDome();
	virtual ~SkyDome();

	virtual void Init(ExecuteValues* exValue);

	virtual void Update(ExecuteValues* exValue);
	virtual void PreRender(ExecuteValues* exValue);
	virtual void Render(ExecuteValues* exValue);
	virtual void PostRender(ExecuteValues* exValue);
	virtual void ResizeScreen(ExecuteValues* exValue);

private:
	typedef Vertex VertexType;
	MeshData<VertexType> *mesh;

	vector<class IComponent*> components;

	SkyDomeBuffer* cBuffer;
};