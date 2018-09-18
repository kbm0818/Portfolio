#include "stdafx.h"
#include "Animator.h"
#include "./Model/Skeleton/ModelSkeleton.h"
#include "./Model/Animation/ModelAnimationController.h"
#include "./Model/Animation/ModelAnimation.h"
#include "./Model/Skeleton/ModelBoneWeights.h"

Animator::Animator() : ComponentBase()
{
	boneBuffer = new ShaderBuffer(&data, sizeof(Data));
	controller = new ModelAnimationController();
	skeleton = nullptr;
}

Animator::~Animator()
{
	SAFE_DELETE(skeleton);
	SAFE_DELETE(controller);
	SAFE_DELETE(boneBuffer);
}

void Animator::Awake()
{
	if (controller->GetCurrentAnimation() == nullptr)
	{
		controller->SetCurrentAnimation(0);
		controller->Play();
	}
}

void Animator::Start()
{
}

void Animator::Update()
{
	if (controller->GetAnimationCount() > 0)
	{
		controller->Update();

		if (skeleton)
		{
			skeleton->UpdateAnimation(controller);

			D3DXMATRIX* bonesArray = skeleton->GetSkinTransforms();
			if (bonesArray)
			{
				UINT boneCount = skeleton->GetBoneCount();
				boneCount = boneCount < ModelBoneWeights::MaxBonesPerMax ? boneCount : ModelBoneWeights::MaxBonesPerMax;

				//버퍼에 boneArray를 셋업한다.
				SetBoneArray(bonesArray, boneCount);
			}
		}
	}
}

void Animator::Render()
{
}

void Animator::CopySkeleton(ModelSkeleton * source)
{
	SAFE_DELETE(skeleton);
	skeleton = new ModelSkeleton(*source);
}

void Animator::SetBoneArray(D3DXMATRIX * matrix, UINT count)
{
	memcpy(data.boneArray, matrix, count * sizeof(D3DXMATRIX));

	for (UINT i = 0; i < count; i++)
		D3DXMatrixTranspose(&data.boneArray[i], &data.boneArray[i]);
}

void Animator::GetBoneMatrix(int index, D3DXMATRIX * mat)
{
	*mat = data.boneArray[index];
}
