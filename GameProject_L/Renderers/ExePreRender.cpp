#include "stdafx.h"
#include "ExePreRender.h"

#include "./Systems/Renders/RenderTarget.h"
#include "./Systems/Camera/Orthographic.h"

ExePreRender::ExePreRender(ExecuteValues* values )
	: Execute(values)
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	renderTarget = new RenderTarget();

	shader = new Shader( Shaders + L"PreRender.hlsl" , ShaderType::VP);
	ortho = new Orthographic
	(
		0, desc.Width, 0, desc.Height
	);

	buffer = new PreRenderBuffer();

	CreateBuffer();
}

ExePreRender::~ExePreRender()
{
	SAFE_DELETE( shader );

	SAFE_RELEASE( indexBuffer );
	SAFE_RELEASE( vertexBuffer );

	SAFE_DELETE( ortho );
	SAFE_DELETE( renderTarget );
}

void ExePreRender::Update()
{
	D3DXMATRIX world;
	D3DXMATRIX S;
	D3DXMatrixScaling( &S, 500, 500, 1 );

	D3DXMATRIX T;
	D3DXMatrixTranslation( &T, 250, 250, 0 );

	world = S * T;

	D3DXMATRIX viewMat;
	D3DXMatrixIdentity( &viewMat );

	D3DXMATRIX projection;
	ortho->GetMatrix( &projection );

	buffer->SetMatrix( world, viewMat, projection );
}

void ExePreRender::PreRender()
{
	ID3D11RenderTargetView* rtv = renderTarget->GetRTV();
	D3D::Get()->SetRenderTarget( rtv );
	D3D::Get()->Clear(rtv, 0xFFFFFFFF);

	if (Keyboard::Get()->Down(VK_SPACE))
	{
		renderTarget->SaveTexture(L"Test.png");
	}

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11ShaderResourceView* view = renderTarget->GetSRV();
	D3D::GetDC()->PSSetShaderResources(0, 1, &view);
	buffer->SetVSBuffer(0);

	shader->Render(ShaderType::VP);

	D3D::GetDC()->DrawIndexed( 6, 0, 0 );
}

void ExePreRender::Render()
{

}

void ExePreRender::PostRender()
{
	
}

void ExePreRender::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc( &desc );
	ortho->Set( 0, desc.Width, 0, desc.Height );
}

void ExePreRender::CreateBuffer()
{
	VertexTexture* vertex = new VertexTexture[ 4 ];

	vertex[ 0 ].position = D3DXVECTOR3( -0.5f, -0.5f, 0.0f );
	vertex[ 1 ].position = D3DXVECTOR3( -0.5f, 0.5f, 0.0f );
	vertex[ 2 ].position = D3DXVECTOR3( 0.5f, -0.5f, 0.0f );
	vertex[ 3 ].position = D3DXVECTOR3( 0.5f, 0.5f, 0.0f );

	vertex[ 0 ].uv = D3DXVECTOR2( 0, 1 );
	vertex[ 1 ].uv = D3DXVECTOR2( 0, 0 );
	vertex[ 2 ].uv = D3DXVECTOR2( 1, 1 );
	vertex[ 3 ].uv = D3DXVECTOR2( 1, 0 );

	UINT* index = new UINT[ 6 ]{ 0,1,2,2,1,3 };

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	ZeroMemory( &desc, sizeof( D3D11_BUFFER_DESC ) );
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof( VertexTexture ) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory( &data, sizeof( D3D11_SUBRESOURCE_DATA ) );
	data.pSysMem = vertex;

	HRESULT hr = D3D::GetDevice()->CreateBuffer( &desc, &data, &vertexBuffer );
	assert( SUCCEEDED( hr ) );

	//2. Index Buffer
	ZeroMemory( &desc, sizeof( D3D11_BUFFER_DESC ) );
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof( UINT ) * 6;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory( &data, sizeof( D3D11_SUBRESOURCE_DATA ) );
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer( &desc, &data, &indexBuffer );
	assert( SUCCEEDED( hr ) );

	SAFE_DELETE_ARRAY( vertex );
	SAFE_DELETE_ARRAY( index );
}
