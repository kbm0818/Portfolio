#pragma once

class Plane
{
public:
	Plane();
	~Plane();

	void Update();
	void Render(Camera* camera);

private:
	typedef VertexColor VertexType;

	Shader* shader;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount;
	UINT indexCount;

	D3DXMATRIX world;
};