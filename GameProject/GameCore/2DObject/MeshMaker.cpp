#include "stdafx.h"
#include "MeshMaker.h"

UINT* MeshMaker::index = nullptr;
UINT MeshMaker::vertexCount = 0;
UINT MeshMaker::indexCount = 0;
ID3D11Buffer* MeshMaker::vertexBuffer = nullptr;
ID3D11Buffer* MeshMaker::indexBuffer = nullptr;


//#include "MeshMaker.h"
//#include "../../Utilities/BinaryMesh.h"
//#include "GameCore\Resource\Mesh.h"
//
//Mesh * MeshMaker::GetPlane(int width, int height, int tilingX, int tilingY)
//{
//	VertexTexture* vertex = NULL;
//	UINT* index = NULL;
//	UINT vertexCount, indexCount;
//	ID3D11Buffer* vertexBuffer;
//	ID3D11Buffer* indexBuffer;
//
//	wstring file = Contents + L"Meshes/Quad.data";
//	BinaryMesh* mesh = new BinaryMesh();
//	{
//		mesh->Open(file);
//		mesh->CopyVertex(&vertex, &vertexCount);
//		mesh->CopyIndex(&index, &indexCount);
//	}
//	SAFE_DELETE(mesh);
//
//	vertex[0].uv = { 0, (float)tilingY };
//	vertex[2].uv = { (float)tilingX,(float)tilingY };
//	vertex[3].uv = { (float)tilingX,0 };
//
//	D3DXMATRIX matRotate, matScale;
//	D3DXMatrixRotationYawPitchRoll(&matRotate, 0, (float)D3DXToRadian(90), 0);
//	D3DXMatrixScaling(&matScale, width, 1, height);
//	for (size_t i = 0; i < vertexCount; ++i)
//	{
//		D3DXVec3TransformCoord(&vertex[i].position, &vertex[i].position, &matRotate);
//		D3DXVec3TransformCoord(&vertex[i].position, &vertex[i].position, &matScale);
//	}
//
//	HRESULT hr;
//
//	D3D11_BUFFER_DESC desc = { 0 };
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
//	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	D3D11_SUBRESOURCE_DATA data = { 0 };
//	data.pSysMem = vertex;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
//	assert(SUCCEEDED(hr));
//
//
//	desc = { 0 };
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(UINT) * indexCount;
//	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//	data = { 0 };
//	data.pSysMem = index;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
//	assert(SUCCEEDED(hr));
//
//	MeshData meshData;
//	meshData.vertexBuffer = vertexBuffer;
//	meshData.indexBuffer = indexBuffer;
//	meshData.offset = 0;
//	meshData.vertexCount = vertexCount;
//	meshData.indexCount = indexCount;
//	meshData.vertexStride = sizeof(VertexTexture);
//
//	Mesh* plane = new Mesh(meshData);
//
//	SAFE_DELETE_ARRAY(vertex);
//	SAFE_DELETE_ARRAY(index);
//
//	return plane;
//}
//
//Mesh * MeshMaker::GetSphere(float radius)
//{
//	VertexTexture* vertex = NULL;
//	UINT* index = NULL;
//	UINT vertexCount, indexCount;
//	ID3D11Buffer* vertexBuffer;
//	ID3D11Buffer* indexBuffer;
//
//	BinaryMesh mesh;
//	mesh.Open(Contents + L"Meshes/Sphere.data");
//	mesh.CopyVertex(&vertex, &vertexCount);
//	mesh.CopyIndex(&index, &indexCount);
//
//	D3DXMATRIX matScale;
//	D3DXMatrixScaling(&matScale, radius, radius, radius);
//	for (size_t i = 0; i < vertexCount; ++i)
//		D3DXVec3TransformCoord(&vertex[i].position, &vertex[i].position, &matScale);
//
//
//	D3D11_BUFFER_DESC desc = { 0 };
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(VertexTexture) * vertexCount;
//	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
//
//	D3D11_SUBRESOURCE_DATA data = { 0 };
//	data.pSysMem = vertex;
//
//	HRESULT hr;
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
//	assert(SUCCEEDED(hr));
//
//
//	desc = { 0 };
//	desc.Usage = D3D11_USAGE_DEFAULT;
//	desc.ByteWidth = sizeof(UINT) * indexCount;
//	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
//
//	data = { 0 };
//	data.pSysMem = index;
//
//	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
//	assert(SUCCEEDED(hr));
//
//	MeshData meshData;
//	meshData.vertexBuffer = vertexBuffer;
//	meshData.indexBuffer = indexBuffer;
//	meshData.offset = 0;
//	meshData.vertexCount = vertexCount;
//	meshData.indexCount = indexCount;
//	meshData.vertexStride = sizeof(VertexTexture);
//
//	Mesh* sphere = new Mesh(meshData);
//
//	SAFE_DELETE_ARRAY(vertex);
//	SAFE_DELETE_ARRAY(index);
//
//	return sphere;
//}
