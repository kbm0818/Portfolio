#include "stdafx.h"
#include "Renderers/Renderers.h"
#include "ModelMesh.h"
#include "Model.h"
#include "ModelPart.h"
#include "ModelMaterial.h"
#include "ModelBoneWeights.h"
#include "ModelBuffer.h"
#include "Object/Components/TransformC.h"

ModelMesh::ModelMesh(ModelPart* modelPart)
	: modelPart(modelPart)
	, material(NULL)
	, vertexData(NULL), indexData(NULL), vertexBuffer(NULL), indexBuffer(NULL)
	, vertexCount(0), indexCount(0)
{
	transform = new TransformC;
	transform->SetSlot(4);

	D3DXMatrixIdentity(&defaultworld);
}

ModelMesh::~ModelMesh()
{
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	SAFE_DELETE(transform);
}

void ModelMesh::Render(GlobalValues* exValue)
{
	if (vertexBuffer == NULL) return;
	if (indexBuffer == NULL) return;
	if (material == NULL) return;
	if (material->CanRender() == false) return;

	transform->SetWorld(world);
	transform->Render(exValue);

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	material->SetPSBuffer(1); // Shader°¡ ·»´õ¸µµÊ

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void ModelMesh::Update(bool isAnimation)
{
	if (isAnimation)
		world = modelPart->GetAnimationTransform();
	else
		world = defaultworld;
}

void ModelMesh::Load(BinaryReader * r, GlobalValues* values)
{
	UINT materialNumber = r->UInt();
	material = modelPart->model->GetMaterial(materialNumber);

	vertexCount = r->UInt();
	vertexData = (VertexType*)malloc(sizeof(VertexType) * vertexCount);
	r->Byte((void**)&vertexData, sizeof(VertexType)* vertexCount);

	indexCount = r->UInt();
	indexData = (UINT*)malloc(sizeof(UINT) * indexCount);
	r->Byte((void**)&indexData, sizeof(UINT)* indexCount);

	isSkinnedModel = r->Bool();

	CreateBuffer();
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
