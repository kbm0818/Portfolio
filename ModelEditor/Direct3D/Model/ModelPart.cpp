#include "../stdafx.h"
#include "ModelPart.h"
#include "ModelMesh.h"
#include "ModelAnimation.h"
#include "ModelAnimationController.h"
#include "ModelAnimationKeyFrames.h"
#include "AnimBuffer.h"
#include "ModelSkeletonBone.h"
#include "ModelSkeleton.h"
#include "Model.h"

ModelPart::ModelPart(string name, AnimBuffer* modelBuffer)
	: name(name), buffer(modelBuffer)
	, isEquip(false), modelSkeletonBone(nullptr)
{
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

ModelPart::ModelPart(AnimBuffer* modelBuffer)
	: buffer(modelBuffer)
	, isEquip(false), modelSkeletonBone(nullptr)
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

void ModelPart::Update(bool isAnimation)
{
	if (isEquip == true)
	{
		matGeometricOffset = model->GetSkeleton()->GetBoneAnimationTransforms(boneName);
	}
	
	for each(ModelMesh* mesh in meshes)
		mesh->Update(isAnimation);
}

void ModelPart::UpdateAnimation(ModelAnimationController * animationController)
{
	if (animationController != NULL)
	{
		ModelAnimation* animation = animationController->GetCurrentAnimation();

		if (animation != NULL)
		{
			ModelAnimationKeyFrames* keyFrames = NULL;
			for each(Pair temp in animationKeyFrames)
			{
				if (temp.first == animation->GetName())
				{
					keyFrames = temp.second;

					break;
				}
			}

			if (keyFrames != NULL)
			{
				int keyFrame = animationController->GetCurrentKeyFrame();
				matAnimationTransform = keyFrames->GetKeyFrameTransform(keyFrame);

				return;
			}
		}
	}

	D3DXMatrixIdentity(&matAnimationTransform);
}

void ModelPart::Render(Camera* camera)
{
	for each(ModelMesh* mesh in meshes)
		mesh->Render(camera);
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

