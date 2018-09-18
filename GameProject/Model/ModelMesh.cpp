#include "stdafx.h"
#include "ModelMesh.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "./ModelBuffer.h"
#include "./Skeleton/ModelBoneWeights.h"
#include "./GameCore/Resource/Mesh.h"


ModelMesh::ModelMesh(UINT materialIndex)
	: vertexBuffer(NULL), indexBuffer(NULL)
	, vertexCount(0), indexCount(0), materialIndex(materialIndex)
{
}

ModelMesh::~ModelMesh()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}
//
//
//void ModelMesh::Render()
//{
//	ID3D11DeviceContext* dc = D3D::GetDC();
//
//	UINT stride = sizeof(VertexTextureNormalBlend);
//	UINT offset = 0;
//
//	dc->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
//	dc->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
//	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//													modelBuffer->SetSkinning(isSkinnedModel);
//	worldBuffer->SetMatrix(world);
//	worldBuffer->SetVSBuffer(WORLD_REGISTER);
//	modelBuffer->SetVSBuffer(2);
//	materialBuffer->SetPSBuffer(1);
//
//	optionBuffer->SetWireFrame(false);
//	optionBuffer->SetPSBuffer(2);
//
//	ID3D11ShaderResourceView* diffuseView = material->GetDiffuseView();
//	dc->PSSetShaderResources(0, 1, &diffuseView);
//
//	shader->Render();
//
//	dc->DrawIndexed(indexCount, 0, 0);
//}

void ModelMesh::CreateBuffer(VertexTextureNormalBlend* vertex, UINT* index)
{
	vertexStride = sizeof(VertexTextureNormalBlend);

	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//1. Vertex Buffer
	if (vertexBuffer != NULL)
		SAFE_RELEASE(vertexBuffer);

	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexTextureNormalBlend) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
	data.pSysMem = vertex;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));

	jointIndex = (int)vertex[0].blendIndices.x;

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
}

void ModelMesh::MoveMeshData(MeshData* data)
{
	data->vertexBuffer = vertexBuffer;
	data->vertexCount = vertexCount;
	data->vertexStride = vertexStride;
	data->indexBuffer = indexBuffer;
	data->indexCount = indexCount;
	data->offset = 0;

	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	indexCount = 0;
	vertexCount = 0;
	vertexStride = 0;
}

