#include "stdafx.h"
#include "D2DRender.h"

#include "Object\Shader\ShaderFactory.h"
#include "Object\Components\TransformC.h"
#include "Systems\Camera\Orthographic.h"

D2DRender::D2DRender(GlobalValues* values)
	: values(values), translation(0, 0), scale(1, 1)
{
	t = new TransformC;

	VertexTexture* vertex = new VertexTexture[4];
	vertex[0].position = D3DXVECTOR3(0, 0, 0);
	vertex[0].uv = D3DXVECTOR2(0, 1);

	vertex[1].position = D3DXVECTOR3(0, 1, 0);
	vertex[1].uv = D3DXVECTOR2(0, 0);

	vertex[2].position = D3DXVECTOR3(1, 0, 0);
	vertex[2].uv = D3DXVECTOR2(1, 1);

	vertex[3].position = D3DXVECTOR3(1, 1, 0);
	vertex[3].uv = D3DXVECTOR2(1, 0);

	UINT* index = new UINT[6]{ 0, 1, 2, 2, 1, 3 };


	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTexture) * 4;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * 6;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);

	SetScale(scale);
	SetTranslation(translation);

	D3DXMatrixIdentity(&view);
}

D2DRender::~D2DRender()
{
	SAFE_DELETE(t);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void D2DRender::Init(ID3D11ShaderResourceView * view, D3DXVECTOR2 pos, D3DXVECTOR2 scale, Shader* shader)
{
	if (shader == nullptr)
		this->shader = values->ShaderFactory->Create(Shaders + L"D2DRender.fx", ShaderType::VP);
	else
		this->shader = shader;

	srv = view;
	SetTranslation(pos);
	SetScale(scale);
}

void D2DRender::Render()
{
	t->SetWorld(world);
	t->Render(values);

	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	D3D::GetDC()->PSSetShaderResources(0, 1, &srv);
	values->RenderOption->SetSampler(SamplerType::CLAMP);

	shader->Render(ShaderType::VP);

	values->RenderOption->TurnOffZBuffer();
	{
		D3D::GetDC()->DrawIndexed(6, 0, 0);
	}
	values->RenderOption->TurnOnZBuffer();
}
