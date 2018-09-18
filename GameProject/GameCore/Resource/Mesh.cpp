#include "stdafx.h"
#include "Mesh.h"
#include "./Model/ModelMesh.h"

Mesh::Mesh(MeshData & meshData)
{
	this->meshData = meshData;
	dc = D3D::GetDC();
}

Mesh::Mesh(ModelMesh * modelMesh)
{
	MeshData meshData;
	modelMesh->MoveMeshData(&meshData);
	this->meshData = meshData;
	dc = D3D::GetDC();
}

Mesh::~Mesh()
{
	this->meshData.Release();
}

void Mesh::BeginRender(D3D_PRIMITIVE_TOPOLOGY topology)
{
	dc->IASetVertexBuffers(0, 1, &meshData.vertexBuffer, &meshData.vertexStride, &meshData.offset);
	dc->IASetIndexBuffer(meshData.indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	dc->IASetPrimitiveTopology(topology);
}

void Mesh::EndRender()
{
	dc->DrawIndexed(meshData.indexCount, 0, 0);
}

void MeshData::Release()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}
