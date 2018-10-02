#include "stdafx.h"
#include "ReflectionWater.h"
#include "./Executes/Execute.h"

ReflectionWater::ReflectionWater()
	:waterTranslation(0.0f), waterHeight(5.0f)
{
	CreateData();
	CreateBuffers();
	CreateTextures();

	D3DXMatrixIdentity(&world);
}

ReflectionWater::~ReflectionWater()
{
	SAFE_DELETE(worldBuffer);
	SAFE_DELETE(vsBuffer);
	SAFE_DELETE(psBuffer);

	SAFE_DELETE(shader);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(normalTexture);
}

void ReflectionWater::Update(ExecuteValues * values)
{
	D3DXMatrixTranslation(&world, 128.0f, waterHeight, 128.0f);

	psBuffer->Data.waterTranslation += 0.003f;
	if (psBuffer->Data.waterTranslation > 1.0f)
		psBuffer->Data.waterTranslation -= 1.0f;

}

void ReflectionWater::Render(ID3D11ShaderResourceView * refractionTexture, ID3D11ShaderResourceView * reflectionTexture)
{
	worldBuffer->SetMatrix(world);
	worldBuffer->SetVSBuffer(1);
	vsBuffer->SetVSBuffer(2);
	psBuffer->SetPSBuffer(1);

	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	shader->Render(ShaderType::VP);

	ID3D11ShaderResourceView* textures[3];
	textures[0] = refractionTexture;
	textures[1] = reflectionTexture;
	textures[2] = normalTexture;

	D3D::GetDC()->PSSetShaderResources(0, 3, textures);
	RenderOptions::Get()->SetSampler(0);

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ReflectionWater::CreateData(float WaterRadius)
{
	vertexCount = indexCount = 6;
	vertex.resize(vertexCount);
	index.resize(indexCount);

	// Load the vertex array with data.
	vertex[0].position = D3DXVECTOR3(-WaterRadius, 0.0f, WaterRadius);  // Top left.
	vertex[0].uv = D3DXVECTOR2(0.0f, 0.0f);

	vertex[1].position = D3DXVECTOR3(WaterRadius, 0.0f, WaterRadius);  // Top right.
	vertex[1].uv = D3DXVECTOR2(1.0f, 0.0f);

	vertex[2].position = D3DXVECTOR3(-WaterRadius, 0.0f, -WaterRadius);  // Bottom left.
	vertex[2].uv = D3DXVECTOR2(0.0f, 1.0f);

	vertex[3].position = D3DXVECTOR3(-WaterRadius, 0.0f, -WaterRadius);  // Bottom left.
	vertex[3].uv = D3DXVECTOR2(0.0f, 1.0f);

	vertex[4].position = D3DXVECTOR3(WaterRadius, 0.0f, WaterRadius);  // Top right.
	vertex[4].uv = D3DXVECTOR2(1.0f, 0.0f);

	vertex[5].position = D3DXVECTOR3(WaterRadius, 0.0f, -WaterRadius);  // Bottom right.
	vertex[5].uv = D3DXVECTOR2(1.0f, 1.0f);

	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 3;
	index[4] = 4;
	index[5] = 5;
}

void ReflectionWater::CreateBuffers()
{
	vsBuffer = new VSReflectionWaterBuffer();
	psBuffer = new PSReflectionWaterBuffer();

	shader = new Shader(Shaders + L"Water.hlsl", ShaderType::VP);
	worldBuffer = new WorldBuffer();
	
	HRESULT hr;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexType) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = &vertex[0];

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	data = { 0 };
	data.pSysMem = &index[0];

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
}

void ReflectionWater::CreateTextures()
{
	Texture::LoadTexture(Textures + L"waternormal.dds", &normalTexture);
}
