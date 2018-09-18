#include "../stdafx.h"
#include "Model.h"
#include "ModelMaterial.h"
#include "ModelPart.h"
#include "AnimBuffer.h"
#include "../Editor.h"

#include "ModelSkeleton.h"
#include "ModelAnimation.h"
#include "ModelAnimationController.h"
#include "ModelAnimationKeyFrames.h"
#include "ModelBoneWeights.h"
#include "ModelSkeletonBone.h"

Model::Model()
	:animationController(NULL)
	, scale(1.0f,1.0f,1.0f)
{
	buffer = new AnimBuffer();
}

Model::~Model()
{
	SAFE_DELETE(buffer);

	for (ModelMaterial* material : materials)
		SAFE_DELETE(material);

	for (ModelPart* part : parts)
		SAFE_DELETE(part);
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

void Model::SetScale(D3DXVECTOR3 scale)
{
	this->scale = scale;

	UpdateScale();
}

void Model::SetScale(float x, float y, float z)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(x, y, z);
	SetScale(temp);

	UpdateScale();
}

void Model::SetScale(float scale)
{
	D3DXVECTOR3 temp = D3DXVECTOR3(scale, scale, scale);
	SetScale(temp);

	UpdateScale();
}

void Model::UpdateScale()
{
	D3DXMATRIX temp;
	D3DXMatrixScaling(&temp, scale.x, scale.y, scale.z);

	buffer->SetBoneScale(temp);
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

			if (skeleton != NULL)
			{
				skeleton->UpdateAnimation(animationController);
			}

			for (ModelPart* part : parts)
				part->UpdateAnimation(animationController);
		}
	}

	if (skeleton != NULL)
	{
		D3DXMATRIX* bonesArray = skeleton->GetSkinTransforms();
		if (bonesArray != NULL)
		{
			UINT boneCount = skeleton->GetBoneCount();
			boneCount = boneCount < ModelBoneWeights::MaxBonesPerMax ? boneCount : ModelBoneWeights::MaxBonesPerMax;

			buffer->SetBoneArray(bonesArray, boneCount);
		}
	}

	buffer->SetSkinning(isAnimated);

	for (ModelPart* part : parts)
	{
		part->Update(isAnimated);
		
	}
}

void Model::Render(Camera * camera)
{
	buffer->SetVSBuffer(2);

	for (ModelPart* part : parts)
		part->Render(camera);
}


///////////////////////////////////////////////////////////////////////////


DLL UINT Cs_GetMaterialCount()
{
	return Editor::Get()->GetModel()->GetMaterialCount();
}

DLL const char* Cs_GetMaterialName(UINT number)
{
	ModelMaterial* material = Editor::Get()->GetModel()->GetMaterial(number);
	return String::ToCSharp(material->GetName());
}

DLL const char* Cs_GetMaterialDiffuseTextureFile(UINT number)
{
	ModelMaterial* material = Editor::Get()->GetModel()->GetMaterial(number);
	return String::ToCSharp(material->GetDiffuseFile());
}

DLL void Cs_SetMaterialDiffuseTextureFile(UINT number, const char* file)
{
	ModelMaterial* material = Editor::Get()->GetModel()->GetMaterial(number);
	material->SetDiffuseTexture(file);
}

DLL const char* Cs_GetMaterialNormalTextureFile(UINT number)
{
	ModelMaterial* material = Editor::Get()->GetModel()->GetMaterial(number);
	return String::ToCSharp(material->GetNormalFile());
}

DLL void Cs_SetMaterialNormalTextureFile(UINT number, const char* file)
{
	ModelMaterial* material = Editor::Get()->GetModel()->GetMaterial(number);
	material->SetNormalTexture(file);
}

DLL const char* Cs_GetMaterialSpecularTextureFile(UINT number)
{
	ModelMaterial* material = Editor::Get()->GetModel()->GetMaterial(number);
	return String::ToCSharp(material->GetSpecularFile());
}

DLL void Cs_SetMaterialSpecularTextureFile(UINT number, const char* file)
{
	ModelMaterial* material = Editor::Get()->GetModel()->GetMaterial(number);
	material->SetSpecularTexture(file);
}

DLL int Cs_GetModelSkeletonSize()
{
	int size = Editor::Get()->GetModel()->GetSkeleton()->GetBoneCount();
	return size;
}

DLL const char* Cs_GetModelSkeletonName(int index)
{
	string boneName = String::WStringToString(Editor::Get()->GetModel()->GetSkeleton()->GetSkeletonBone(index)->GetName());
	return String::ToCSharp(boneName);
}

DLL int Cs_GetModelPartSize()
{
	int size = (int)(Editor::Get()->GetModel()->GetModelPartSize());
	return size;
}

DLL const char* Cs_GetAnimationName(int index)
{
	string name = Editor::Get()->GetModel()->GetAnimationController()->GetAnimationName(index);
	return String::ToCSharp(name);
}

DLL int Cs_GetAnimationSize()
{
	int size = (int)(Editor::Get()->GetModel()->GetAnimationController()->GetAnimationCount());
	return size;
}

DLL const char* Cs_GetModelPartName(int index)
{
	string name = Editor::Get()->GetModel()->GetModelPartName(index);
	return String::ToCSharp(name);
}

DLL int Cs_DeleteModelPart(int index)
{
	bool result = Editor::Get()->GetModel()->DeleteModelPart(index);

	if (result == true)
		return 1;
	else
		return 0;
}

DLL void Cs_DeleteAnimation(int index)
{
	Editor::Get()->GetModel()->DeleteAnimation((UINT)index);
}

DLL void Cs_PlayAnimation(int index)
{
	Editor::Get()->GetModel()->PlayAnimation((UINT)index);
}

DLL float Cs_KeyFrameFactor()
{
	float temp = Editor::Get()->GetModel()->GetAnimationController()->GetKeyFrameFactor();
	return 1.0f - temp;
}

