#include "stdafx.h"
#include "Model.h"
#include "ModelPart.h"
#include "ModelMesh.h"
#include "ModelAnimation.h"
#include "ModelAnimationController.h"
#include "ModelAnimationKeyFrames.h"
#include "ModelBuffer.h"
#include "ModelSkeletonBone.h"
#include "ModelSkeleton.h"

ModelPart::ModelPart(string name)
	: name(name)
	, isEquip(false), modelSkeletonBone(nullptr)
{
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

ModelPart::ModelPart(Model* model)
	: isEquip(false), modelSkeletonBone(nullptr), model(model)
{
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

ModelPart::~ModelPart()
{
	for each(Pair temp in animationKeyFrames)
		SAFE_DELETE(temp.second);

	for (size_t i = 0; i < meshes.size(); i++)
		SAFE_DELETE(meshes[i]);
}

void ModelPart::Load(BinaryReader * r, GlobalValues* values)
{
	name = r->String();

	UINT count = r->UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh(this);
		mesh->Load(r, values);
		meshes.push_back(mesh);
	}
}

void ModelPart::Update(bool isAnimation)
{
	if (isEquip == true)
	{
		matGeometricOffset = model->GetSkeleton()->GetBoneAnimationTransforms(boneName);
	}
	
	for each(ModelMesh* mesh in meshes)
		mesh->Update(isAnimation);
}

void ModelPart::Render(GlobalValues* exValue)
{
	for each(ModelMesh* mesh in meshes)
		mesh->Render(exValue);
}

void ModelPart::AddAnimationKeyFrames(ModelAnimationKeyFrames * animationKeyFrames)
{
	this->animationKeyFrames.push_back(Pair(animationKeyFrames->GetAnimationName(), animationKeyFrames));
}

ModelAnimationKeyFrames * ModelPart::GetAnimationKeyFrames(wstring animationName)
{
	for each(Pair temp in animationKeyFrames)
	{
		if (temp.first == animationName)
			return temp.second;
	}

	return NULL;
}

