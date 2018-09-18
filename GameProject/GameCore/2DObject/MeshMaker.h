#pragma once
#include "GameCore\Resource\Mesh.h"
#include "Utilities\BinaryMesh.h"

class MeshMaker
{
public:
	MeshMaker(){}
	~MeshMaker() {}

	template <typename T>
	static Mesh* GetPlane(int width = 10, int height = 10, int tilingX = 1, int tilingY = 1);
	template <typename T>
	static Mesh* GetPlane(wstring file);
	template <typename T>
	static Mesh* GetSphere(float radius = 1);
	template <typename T>
	static Mesh* GetCube(float diameter = 1);

private:
	static UINT* index;
	static UINT vertexCount;
	static UINT indexCount;
	static ID3D11Buffer* vertexBuffer;
	static ID3D11Buffer* indexBuffer;

	template <typename VertexType>
	static void LoadBinaryMesh(wstring file, VertexType** vertex);

	template <typename VertexType>
	static void LoadBinaryTerrain(wstring file, VertexType** vertex);

	template <typename T>
	static void CreateBuffer(T* vertex);
	template <typename T>
	static Mesh* GetMesh(T* vertex);
	template <typename T>
	static void SetSRT(T* vertex, D3DXVECTOR3 transform, D3DXVECTOR3 rotation, D3DXVECTOR3 scale);
	static void Release()
	{
		vertexBuffer = nullptr;
		indexBuffer = nullptr;
		SAFE_DELETE_ARRAY(index);
	}
};

template <typename T>
static Mesh* MeshMaker::GetPlane(int width, int height, int tilingX, int tilingY)
{
	T* vertex = NULL;
	LoadBinaryMesh<T>(Contents + L"Meshes/Quad.data", &vertex);

	vertex[0].uv = { 0, (float)tilingY };
	vertex[2].uv = { (float)tilingX,(float)tilingY };
	vertex[3].uv = { (float)tilingX,0 };

	SetSRT<T>(vertex, D3DXVECTOR3(0,0,0), D3DXVECTOR3((float) 90, 0, 0 ), D3DXVECTOR3((float)width,1,(float)height ));
	CreateBuffer<T>(vertex);
	
	Mesh* mesh = GetMesh<T>(vertex);

	Release();
	SAFE_DELETE_ARRAY(vertex);
	return mesh;
}

template <typename T>
static Mesh* MeshMaker::GetPlane(wstring file)
{
	T* vertex = NULL;

	LoadBinaryTerrain<T>(Contents + file, &vertex);
	SetSRT<T>(vertex, { 0,0,0 }, { 0, 0, 0 }, { radius,radius,radius });

	CreateBuffer<T>(vertex);
	Mesh* mesh = GetMesh<T>(vertex);

	Release();
	SAFE_DELETE_ARRAY(vertex);
	return mesh;
}

template <typename T>
static Mesh* MeshMaker::GetSphere(float radius)
{
	T* vertex = NULL;

	LoadBinaryMesh<T>(Contents + L"Meshes/Sphere.data", &vertex);
	SetSRT<T>(vertex, { 0,0,0 }, { 0, 0, 0 }, { radius,radius,radius });

	CreateBuffer<T>(vertex);
	Mesh* mesh = GetMesh<T>(vertex);

	Release();
	SAFE_DELETE_ARRAY(vertex);
	return mesh;
}

template <typename T>
static Mesh* MeshMaker::GetCube(float diameter)
{
	T* vertex = NULL;

	LoadBinaryMesh<T>(Contents + L"Meshes/Cube.data", &vertex);
	SetSRT<T>(vertex, { 0,0,0 }, { 0, 0, 0 }, { diameter,diameter,diameter });

	CreateBuffer<T>(vertex);
	Mesh* mesh = GetMesh<T>(vertex);

	Release();
	SAFE_DELETE_ARRAY(vertex);
	return mesh;
}


template <typename VertexType>
static void MeshMaker::LoadBinaryMesh(wstring file, VertexType** vertex)
{
	BinaryMesh* mesh = new BinaryMesh();
	{
		mesh->Open(file);
		mesh->CopyVertex(vertex, &vertexCount);
		mesh->CopyIndex(&index, &indexCount);
	}
	SAFE_DELETE(mesh);
}

template <typename VertexType>
static void MeshMaker::LoadBinaryTerrain(wstring file, VertexType** vertex)
{

	//TODO:: 수정요망 vertex, vertexCount, index, indexCount까지
}


template <typename T>
static void MeshMaker::CreateBuffer(T* vertex)
{
	HRESULT hr;


	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(T) * vertexCount;
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
}

template <typename T>
static Mesh* MeshMaker::GetMesh(T* vertex)
{
	MeshData meshData;
	meshData.vertexBuffer = vertexBuffer;
	meshData.indexBuffer = indexBuffer;
	meshData.offset = 0;
	meshData.vertexCount = vertexCount;
	meshData.indexCount = indexCount;
	meshData.vertexStride = sizeof(T);

	Mesh* m = new Mesh(meshData); 
	return m;
}


template <typename T>
static void MeshMaker::SetSRT(T* vertex, D3DXVECTOR3 transform, D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	D3DXMATRIX matRotate, matScale, matTrans;
	D3DXMatrixTranslation(&matTrans, transform.x, transform.y, transform.z);
	D3DXMatrixRotationYawPitchRoll(&matRotate, (float)D3DXToRadian(rotation.y), (float)D3DXToRadian(rotation.x), (float)D3DXToRadian(rotation.z));
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	for (size_t i = 0; i < vertexCount; ++i)
	{
		D3DXVec3TransformCoord(&vertex[i].position, &vertex[i].position, &matTrans);
		D3DXVec3TransformCoord(&vertex[i].position, &vertex[i].position, &matRotate);
		D3DXVec3TransformCoord(&vertex[i].position, &vertex[i].position, &matScale);
	}
}
