#include "../stdafx.h"
#include "ModelMesh.h"
#include "ModelPart.h"
#include "ModelMaterial.h"
#include "ModelBoneWeights.h"
#include "ModelBuffer.h"

ModelMesh::ModelMesh(ModelPart* modelPart, AnimBuffer* modelBuffer)
	: modelPart(modelPart), modelBuffer(modelBuffer)
	, material(NULL)
	, vertexData(NULL), indexData(NULL), vertexBuffer(NULL), indexBuffer(NULL)
	, vertexCount(0), indexCount(0)
{

}

ModelMesh::~ModelMesh()
{
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void ModelMesh::Update(bool isAnimation)
{
	if (isAnimation == true)
	{
		if (isSkinnedModel == true)
			world = modelPart->GetGeometricOffset();
		else
			world = modelPart->GetGeometricOffset() * modelPart->GetAnimationTransform();
	}
	else
		world = modelPart->GetGeometricOffset();
}

void ModelMesh::Render(Camera * camera)
{
	if (vertexBuffer == NULL) return;
	if (indexBuffer == NULL) return;
	if (material == NULL) return;
	if (material->CanRender() == false) return;

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->SetPSBuffer(); // Shader°¡ ·»´õ¸µµÊ
	camera->SetVSBuffer(&world);

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ModelMesh::CreateBuffer()
{
	HRESULT hr;
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
	data.pSysMem = vertexData;

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
	data.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
}
