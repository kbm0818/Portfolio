#pragma once
#include "Execute.h"

class PreRenderBuffer : public ShaderBuffer
{
public:
	PreRenderBuffer() : ShaderBuffer( &Data, sizeof( Data ) )
	{
		D3DXMatrixIdentity( &Data.world );
		D3DXMatrixIdentity( &Data.view );
		D3DXMatrixIdentity( &Data.projection );
	}

	void SetMatrix( D3DXMATRIX world, D3DXMATRIX view, D3DXMATRIX projection )
	{
		D3DXMatrixTranspose( &Data.world, &world );
		D3DXMatrixTranspose( &Data.view, &view );
		D3DXMatrixTranspose( &Data.projection, &projection );
	}

	struct Struct
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	} Data ;
};

class ExePreRender : public Execute
{
public:
	ExePreRender( ExecuteValues* values );
	~ExePreRender();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	void CreateBuffer();

	class RenderTarget* renderTarget;

	Shader* shader;
	class Orthographic* ortho;
	PreRenderBuffer* buffer;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

};