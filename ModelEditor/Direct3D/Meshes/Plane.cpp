#include "../stdafx.h"
#include "Plane.h"
#include "../Utilities/Mesh.h"

Plane::Plane()
{
	wstring file = Shaders + L"VertexColor.hlsl";
	shader = new Shader(file);
	shader->CreateInputLayout(VertexType::desc, VertexType::count);

	VertexType* vertex = NULL;
	UINT* index = NULL;

	file = Contents + L"Meshes/Quad.data";
	Mesh* mesh = new Mesh();
	{
		mesh->Open(file);
		mesh->CopyVertex(&vertex, &vertexCount, D3DXCOLOR(0xFFD1D5DD));
		mesh->CopyIndex(&index, &indexCount);
	}
	SAFE_DELETE(mesh);

	HRESULT hr;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexType) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	data = { 0 };
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);

	
	D3DXMATRIX S, R, T;
	D3DXMatrixScaling(&S, 50, 50, 1);
	D3DXMatrixRotationX(&R, 90 * (float)D3DX_PI / 180);
	D3DXMatrixTranslation(&T, -25.0f, 0.0f, -25.0f);
	world = S * R * T;
}

Plane::~Plane()
{
	SAFE_DELETE(shader);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Plane::Update()
{
}

void Plane::Render(Camera * camera)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	camera->SetVSBuffer(&world);
	shader->Render();

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}
