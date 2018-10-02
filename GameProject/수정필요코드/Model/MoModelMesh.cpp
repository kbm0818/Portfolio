#include "stdafx.h"
#include "MoModelMesh.h"
#include "MoModel.h"
#include "MoMaterial.h"
#include "MoBoneWeights.h"
#include "Object/Model/Model.h"
#include "Object/Model/ModelMesh.h"
#include "Object/Model/ModelMaterial.h"
#include "Object/Model/ModelPart.h"

MoModelMesh::MoModelMesh(MoPart * part, MoMaterial * material)
	: part(part), material(material)
{
}

MoModelMesh::~MoModelMesh()
{
}

void MoModelMesh::AddVertex(D3DXVECTOR3 & position, D3DXVECTOR3 & normal, D3DXVECTOR2 & uv, const MoBoneWeights& boneWeight)
{
	VertexType vertex;
	vertex.position = position;
	vertex.uv = uv;
	vertex.normal = normal;
	vertices.push_back(vertex);

	indices.push_back((UINT)indices.size());

	boneWeights.push_back(boneWeight);
	if(boneWeight.GetBoneWeightCount() > 0)
		isSkinnedModel = true;
}

void MoModelMesh::AddVertex(D3DXVECTOR3 & position, D3DXVECTOR3 & normal, D3DXVECTOR2 & uv)
{
	VertexType vertex;
	vertex.position = position;
	vertex.uv = uv;
	vertex.normal = normal;
	vertices.push_back(vertex);

	indices.push_back((UINT)indices.size());
}

void MoModelMesh::Write(BinaryWriter * w)
{
	CalcTBB();

	UINT materialNumber = material->GetNumber();
	w->UInt(materialNumber);

	w->UInt(vertices.size());
	w->Byte(&vertices[0], sizeof(VertexType) * vertices.size());

	w->UInt(indices.size());
	w->Byte(&indices[0], sizeof(UINT) * indices.size());

	w->Bool(isSkinnedModel);
}

void MoModelMesh::Read(BinaryReader * r, Model* model, ModelMesh * modelMesh)
{
	UINT materialNumber = r->UInt();
	ModelMaterial* material = model->GetMatchMaterial(materialNumber);
	modelMesh->SetMaterial(material);

	modelMesh->vertexCount = r->UInt();
	modelMesh->vertexData = new VertexType[modelMesh->vertexCount];
	r->Byte((void **)&modelMesh->vertexData, sizeof(VertexType) * modelMesh->vertexCount);

	modelMesh->indexCount = r->UInt();
	modelMesh->indexData = new UINT[modelMesh->indexCount];
	r->Byte((void **)&modelMesh->indexData, sizeof(UINT) * modelMesh->indexCount);

	modelMesh->isSkinnedModel = r->Bool();

	modelMesh->CreateBuffer();
}

void MoModelMesh::Write(BinaryWriter * w, ModelMesh * modelMesh)
{
	/*UINT materialNumber = modelMesh->material->GetNumber();
	w->UInt(materialNumber);

	w->UInt(modelMesh->vertexCount);
	w->Byte(modelMesh->vertexData, sizeof(VertexTextureNormal) * modelMesh->vertexCount);

	w->UInt(modelMesh->indexCount);
	w->Byte(modelMesh->indexData, sizeof(UINT) * modelMesh->indexCount);*/
}

void MoModelMesh::CalcTBB()
{
	for (UINT i = 0; i < boneWeights.size(); i++)
	{
		MoBlendWeights weight = boneWeights[i].GetBlendWeights();
		vertices[i].blendIndices = weight.BlendIndices;
		vertices[i].blendWeights = weight.BlendWeights;
	}

	///// ¿©±â¼­ ÅºÁ¨Æ® °è»ê

	for (UINT i = 0; i < ((UINT)(indices.size()) / 3); i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexTextureNormalTangentBinormalBlend& vertex0 = vertices[index0];
		VertexTextureNormalTangentBinormalBlend& vertex1 = vertices[index1];
		VertexTextureNormalTangentBinormalBlend& vertex2 = vertices[index2];

		D3DXVECTOR3 e0 = vertex1.position - vertex0.position;
		D3DXVECTOR3 e1 = vertex2.position - vertex0.position;

		float u0 = vertex1.uv.x - vertex0.uv.x;
		float u1 = vertex2.uv.x - vertex0.uv.x;

		float v0 = vertex1.uv.y - vertex0.uv.y;
		float v1 = vertex2.uv.y - vertex0.uv.y;

		float r = 1.0f / (u0 * v1 - v0 * u1);

		D3DXVECTOR3 tangent;
		tangent.x = r * (v1 * e0.x - v0 * e1.x);
		tangent.y = r * (v1 * e0.y - v0 * e1.y);
		tangent.z = r * (v1 * e0.z - v0 * e1.z);

		D3DXVECTOR3 binormal;
		binormal.x = r * (-u1 * e0.x + u0 * e1.x);
		binormal.y = r * (-u1 * e0.y + u0 * e1.y);
		binormal.z = r * (-u1 * e0.z + u0 * e1.z);

		vertex0.tangent += tangent;
		vertex1.tangent += tangent;
		vertex2.tangent += tangent;

		vertex0.binormal += binormal;
		vertex1.binormal += binormal;
		vertex2.binormal += binormal;
	}
	////////////////ÅºÁ¨Æ® °è»ê ³¡

	for (UINT i = 0; i < vertices.size(); i++)
	{
		D3DXVec3Normalize(&vertices[i].tangent, &vertices[i].tangent);
		D3DXVec3Normalize(&vertices[i].binormal, &vertices[i].binormal);
	}
}
