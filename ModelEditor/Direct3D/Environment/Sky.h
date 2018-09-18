#pragma once

class SkyBuffer;
class Sky
{
public:
	Sky();
	~Sky();

	void SetShader(wstring file);

	void Update(Camera* camera);
	void Render(Camera* camera);

private:
	typedef Vertex VertexType;

	wstring file;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount;
	UINT indexCount;

	Shader* shader;
	SkyBuffer* skyBuffer;

	D3DXMATRIX world;

	ID3D11RasterizerState* clockState;
	ID3D11RasterizerState* countClockState;
	ID3D11DepthStencilState* depthOnState;
	ID3D11DepthStencilState* depthOffState;
};