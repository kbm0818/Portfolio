#include "../stdafx.h"
#include "MoPart.h"
#include "MoModelMesh.h"
#include "../Model/ModelPart.h"
#include "../Model/ModelMesh.h"
#include "../Model/ModelBuffer.h"

MoPart::MoPart(string name)
	: name(name)
{
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

MoPart::MoPart()
{
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

MoPart::~MoPart()
{
	for (size_t i = 0; i < meshes.size(); i++)
		SAFE_DELETE(meshes[i]);
}

void MoPart::AddVertex(MoMaterial * material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv, const MoBoneWeights& boneWeight)
{
	bool isNew = true;

	MoModelMesh* mesh = NULL;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		mesh = meshes[i];

		if (material == mesh->GetMaterial())
		{
			mesh->AddVertex(position, normal, uv, boneWeight);
			isNew = false;
		}
	}

	if (isNew == true)
	{
		mesh = new MoModelMesh(this, material);
		mesh->AddVertex(position, normal, uv, boneWeight);

		meshes.push_back(mesh);
	}
}

void MoPart::AddVertex(MoMaterial * material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv)
{
	bool isNew = true;

	MoModelMesh* mesh = NULL;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		mesh = meshes[i];

		if (material == mesh->GetMaterial())
		{
			mesh->AddVertex(position, normal, uv);
			isNew = false;
		}
	}

	if (isNew == true)
	{
		mesh = new MoModelMesh(this, material);
		mesh->AddVertex(position, normal, uv);

		meshes.push_back(mesh);
	}
}

void MoPart::Write(BinaryWriter * w)
{
	w->String(name);

	w->UInt(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Write(w);
	}
}

void MoPart::Read(BinaryReader * r, Model * model, ModelPart * part)
{
	part->name = r->String();
	UINT size = r->UInt();
	
	for (UINT i = 0; i < size; i++)
	{
		ModelMesh* temp = new ModelMesh(part, model->buffer);
		MoModelMesh::Read(r, model, temp);
		part->meshes.push_back(temp);
	}
}

void MoPart::Write(BinaryWriter * w, ModelPart * modelMesh)
{
}

