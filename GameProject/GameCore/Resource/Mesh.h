#pragma once

struct MeshData
{
	ID3D11Buffer * vertexBuffer;
	UINT vertexCount;
	UINT vertexStride;
	UINT offset;

	ID3D11Buffer* indexBuffer;
	UINT indexCount;

	void Release();
};

class ModelMesh;
class Mesh
{
public:
	Mesh(MeshData& meshData);
	Mesh(ModelMesh* modelMesh);
	~Mesh();

	void BeginRender(D3D_PRIMITIVE_TOPOLOGY topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void EndRender();

private:
	MeshData meshData;
	ID3D11DeviceContext* dc;
};