#include "stdafx.h"
#include "Capsule2D.h"

Capsule2D::Capsule2D(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
	: position(pos),
	indexBuffer(NULL), vertexBuffer(NULL), shader(NULL)
{
	SetRotation(rot);

	//기본 VertexColor 쉐이더
	shader = AssetManager::Get()->GetShader("VertexColor");

	D3DXMatrixTransformation(&world,
		NULL, NULL, NULL, NULL, &rotation, &position);
	CreateBuffer();

	worldBuffer = new WorldBuffer();
}

Capsule2D::Capsule2D(D3DXVECTOR3 pos, D3DXQUATERNION rot)
	: position(pos), rotation(rot),
	indexBuffer(NULL), vertexBuffer(NULL), shader(NULL)
{
	//기본 VertexColor 쉐이더
	shader = AssetManager::Get()->GetShader("VertexColor");

	D3DXMatrixTransformation(&world,
		NULL, NULL, NULL, NULL, &rotation, &position);
	CreateBuffer();

	worldBuffer = new WorldBuffer();
}

Capsule2D::~Capsule2D()
{
	SAFE_DELETE(worldBuffer);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Capsule2D::Update()
{
	D3DXMatrixTransformation(&world,
		NULL, NULL, NULL, NULL, &rotation, &position);
}

void Capsule2D::Render()
{
	ID3D11DeviceContext* dc = D3D::GetDC();
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	worldBuffer->SetMatrix(world);
	worldBuffer->SetVSBuffer(WORLD_REGISTER);
	shader->Render();

	//States::SetDepthOff();
	dc->DrawIndexed(indexCount, 0, 0);
	//States::SetDepthDefault();
}

void Capsule2D::UpdateVertex(float radius, float height)
{
	HRESULT hr;
	VertexType* vertex;

	//정점 정보 생성
	vertexCount = 361;
	indexCount = 361;

	vertex = new VertexType[vertexCount];

	//x,y 평면 (sin: y, cos: x)
	for (UINT i = 0; i < vertexCount; ++i)
	{
		float radian = (float)D3DXToRadian(i);
		float offset = (i < 180) ? height*0.5f : -height * 0.5f;
		if (i >= 360) offset = height * 0.5f;

		vertex[i].position = D3DXVECTOR3(cos(radian)*radius, sin(radian)*radius + offset, 0);
		vertex[i].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//버퍼 생성
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	if (vertexBuffer != NULL)
		SAFE_RELEASE(vertexBuffer);

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexType) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));
}

void Capsule2D::CreateBuffer()
{
	HRESULT hr;
	VertexType* vertex;
	UINT* index;

	//정점 정보 생성
	vertexCount = 361;
	indexCount = 361;

	vertex = new VertexType[vertexCount];
	index = new UINT[indexCount];

	//x,y 평면 (sin: y, cos: x)
	for (UINT i = 0; i < vertexCount; ++i)
	{
		float radian = (float)D3DXToRadian(i);
		float offset = (i < 180) ? 1.0f : -1.0f;
		if (i >= 360) offset = 1.0f;

		vertex[i].position = D3DXVECTOR3(cos(radian), sin(radian) + offset, 0);
		vertex[i].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	for (UINT i = 0; i < indexCount; ++i)
		index[i] = i;


	//버퍼 생성
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	if (vertexBuffer != NULL)
		SAFE_RELEASE(vertexBuffer);

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexType) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	//2. Index Buffer
	if (indexBuffer != NULL)
		SAFE_RELEASE(indexBuffer);

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(index);
	SAFE_DELETE_ARRAY(vertex);
}
