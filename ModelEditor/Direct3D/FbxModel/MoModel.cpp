#include "../stdafx.h"
#include "MoModel.h"
#include "MoMaterial.h"
#include "MoModelMesh.h"
#include "../Model/Model.h"
#include "../Model/ModelMaterial.h"
#include "../Model/ModelMesh.h"
#include "../Model/ModelSkeleton.h"
#include "../Model/ModelPart.h"
#include "MoAnimation.h"
#include "MoAnimationController.h"
#include "MoAnimationKeyFrames.h"
#include "MoSkeleton.h"
#include "MoSkeletonBone.h"
#include "MoBoneWeights.h"
#include "MoPart.h"

#include "MoModelUtility.h"

MoModel::MoModel(string file)
	: file(file)
{
	D3DXMatrixIdentity(&geometricOffset);
}

MoModel::~MoModel()
{
	for (MoMaterial* material : materials)
		SAFE_DELETE(material);
}

void MoModel::PushMaterial(MoMaterial * material)
{
	materials.push_back(material);
}

D3DXMATRIX MoModel::GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if (node == NULL)
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}

	/*FbxAnimEvaluator* e = node->GetAnimationEvaluator();
	FbxAMatrix matrix = e->GetNodeGlobalTransform(node, time, FbxNode::eSourcePivot, false);*/
	FbxAMatrix matrix = node->EvaluateGlobalTransform(time);

	return MoModelUtility::ToMatrix(matrix);
}

void MoModel::Write(string file)
{
	BinaryWriter* w = new BinaryWriter();
	wstring temp = String::StringToWString(file);

	w->Open(temp);
	{
		w->Matrix(geometricOffset);

		w->UInt(materials.size());
		for (MoMaterial* material : materials)
			material->Write(w);

		w->UInt(parts.size());
		for (MoPart* part : parts)
			part->Write(w);

		skeleton->Write(w);
	}
	w->Close();

	SAFE_DELETE(w);
}

void MoModel::Read(string file, Model ** model)
{
	BinaryReader* r = new BinaryReader();
	wstring temp = String::StringToWString(file);
	
	UINT count = 0;
	r->Open(temp);
	{
		*model = new Model();
		(*model)->matGeometricOffset = r->Matrix();

		count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{		
			ModelMaterial* material = new ModelMaterial();
			MoMaterial::Read(r, material);

			(*model)->materials.push_back(material);
		}

		count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{
			ModelPart* part = new ModelPart((*model)->buffer);
			MoPart::Read(r, *model, part);

			(*model)->parts.push_back(part);
		}

		(*model)->skeleton = new ModelSkeleton();
		MoSkeleton::Read(r, (*model)->skeleton);
	}
	r->Close();

	SAFE_DELETE(r);
}

void MoModel::ReadParts(string file, Model ** model)
{
	BinaryReader* r = new BinaryReader();
	wstring temp = String::StringToWString(file);

	UINT count = 0;
	r->Open(temp);
	{
		D3DXMATRIX temp = r->Matrix();

		count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{
			ModelMaterial* material = new ModelMaterial();
			MoMaterial::Read(r, material);

			(*model)->materials.push_back(material);

			material->number = (*model)->materials.size() - 1;
		}

		count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{
			ModelPart* part = new ModelPart((*model)->buffer);
			MoPart::Read(r, *model, part);
			part->model = (*model);

			(*model)->parts.push_back(part);
		}



	}
	r->Close();

	SAFE_DELETE(r);
}

void MoModel::Write(string file, Model * model)
{
	//BinaryWriter* w = new BinaryWriter();
	//wstring temp = String::StringToWString(file);

	//w->Open(temp);
	//{
	//	w->Matrix(model->matGeometricOffset);

	//	w->UInt(model->materials.size());
	//	for (ModelMaterial* material : model->materials)
	//		MoMaterial::Write(w, material);

	//	w->UInt(model->meshes.size());
	//	for (MoPart* part : model->parts)
	//		MoPart::Write(w, mesh);

	//	MoSkeleton::Write(w, model->skeleton);
	//}
	//w->Close();

	//SAFE_DELETE(w);
}
