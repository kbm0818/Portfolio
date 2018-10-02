#pragma once
#include "./Renders/Renderer.h"

class Billboard : public Renderer<VertexTexture>
{
public:
	Billboard(wstring textureFile);
	virtual ~Billboard();

	virtual void Update(struct ExecuteValues* exValue);
	void Render();

	void SetPosition(D3DXVECTOR3& pos);
	void GetPosition(D3DXVECTOR3& pos);

private:
	virtual void CreateData();
	void CreateTexture();

private:
	Shader* shader;
	WorldBuffer* worldBuffer;

	wstring textureFile;
	ID3D11ShaderResourceView *texture;

	D3DXVECTOR3 scale;
};