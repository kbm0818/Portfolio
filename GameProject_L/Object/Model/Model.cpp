#include "stdafx.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelPart.h"
#include "ModelBuffer.h"

#include "ModelSkeleton.h"
#include "ModelAnimation.h"
#include "ModelAnimationController.h"
#include "ModelAnimationKeyFrames.h"
#include "ModelBoneWeights.h"
#include "ModelSkeletonBone.h"

#include "../Components/Transform_PhysiscC.h"

Model::Model()
	:animationController(nullptr)
{
}

Model::~Model()
{
	for (ModelMaterial* material : materials)
		SAFE_DELETE(material);

	for (ModelPart* part : parts)
		SAFE_DELETE(part);

	SAFE_DELETE(skeleton);
	SAFE_DELETE(animationController);
}

void Model::Update(GlobalValues * values)
{
	Update();
}

void Model::Draw(GlobalValues * values, D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	skeleton->GetBuffer()->SetVSBuffer(3);

	for (ModelPart* part : parts)
		part->Render(values);
}

void Model::Load(wstring modelName, GlobalValues* values)
{
	BinaryReader* r = new BinaryReader;
	wstring tempPath = Contents + L"UserBinaryFbx/" + modelName + L"/" + modelName;
	r->Open(tempPath + L".mat");
	UINT count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMaterial* mat = new ModelMaterial();
		mat->Load(r, values);
		materials.push_back(mat);
	}
	r->Close();

	r->Open(tempPath + L".msh");
	count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{ 
		ModelPart* part = new ModelPart(this);
		part->Load(r, values);
		parts.push_back(part);
	}
	r->Close();

	r->Open(tempPath + L".skl");
	skeleton = new ModelSkeleton;
	skeleton->Load(r, values);
	r->Close();

	SAFE_DELETE(r);
}

D3DXMATRIX Model::GetGeometricOffset()
{
	return D3DXMATRIX();
}

ModelMaterial * Model::GetMatchMaterial(UINT number)
{
	for (ModelMaterial* material : materials)
	{
		if (material->GetNumber() == number)
			return material;
	}

	return NULL;
}

string Model::GetModelPartName(UINT index)
{
	 return parts[index]->GetName(); 
}

bool Model::DeleteModelPart(UINT index)
{
	if (parts.size() < index)
		return false;

	SAFE_DELETE(parts[index]);
	parts.erase(parts.begin() + index);
	return true;
}

void Model::DeleteAnimation(UINT index)
{
	skeleton->DeleteAnimation(index);
	animationController->DeleteAnimation(index);
}

void Model::PlayAnimation(UINT index)
{
	if(animationController->GetAnimationCount() > index)
		animationController->SetCurrentAnimation(index);
}

ModelPart * Model::GetModelPart(string name)
{
	for (size_t i = 0; i < parts.size(); i++)
	{
		string partName = parts[i]->GetName();

		if (strcmp(partName.c_str(), name.c_str()) == 0)
		{
			return parts[i];
		}
	}

	return nullptr;
}

void Model::Update()
{
	bool isAnimated = false;
	if (animationController != NULL)
	{
		isAnimated = animationController->GetAnimationCount() > 0;

		if (isAnimated == true)
		{
			animationController->Update();

			skeleton->UpdateAnimation(animationController);
		}
	}

	D3DXMATRIX* bonesArray = skeleton->GetSkinTransforms();
	if (bonesArray != NULL)
	{
		UINT boneCount = skeleton->GetBoneCount();
		boneCount = boneCount < ModelBoneWeights::MaxBonesPerMax ? boneCount : ModelBoneWeights::MaxBonesPerMax;

		skeleton->GetBuffer()->SetBoneArray(bonesArray, boneCount);

		skeleton->GetBuffer()->SetSkinning(isAnimated);
	}
}

void Model::Render(GlobalValues* exValue)
{
	skeleton->GetBuffer()->SetVSBuffer(3);

	for (ModelPart* part : parts)
		part->Render(exValue);
}