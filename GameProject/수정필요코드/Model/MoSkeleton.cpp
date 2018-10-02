#include "stdafx.h"
#include "MoSkeleton.h"
#include "MoSkeletonBone.h"
#include "Object/Model/ModelSkeleton.h"
#include "Object/Model/ModelSkeletonBone.h"
#include "MoAnimation.h"
#include "MoAnimationController.h"
#include "MoAnimationKeyFrames.h"

MoSkeleton::MoSkeleton()
{
	skinTransforms = NULL;
	boneAnimationTransforms = NULL;

	boneCount = 0;
}

MoSkeleton::~MoSkeleton()
{
	for each(Pair temp in skeletonBones)
		SAFE_DELETE(temp.second);

	SAFE_DELETE_ARRAY(boneAnimationTransforms);
	SAFE_DELETE_ARRAY(skinTransforms);
}

void MoSkeleton::AddSkeletonBone(MoSkeletonBone * skeletonBone)
{
	skeletonBones.push_back(Pair(skeletonBone->GetName(), skeletonBone));
	
	boneCount++;
}

MoSkeletonBone * MoSkeleton::FindBone(wstring boneName)
{
	for each(Pair temp in skeletonBones)
	{
		if (boneName == temp.first)
			return temp.second;
	}
	
	return NULL;
}

int MoSkeleton::FindBoneIndex(wstring boneName)
{
	int index = 0;
	for each(Pair temp in skeletonBones)
	{
		if (boneName == temp.first)
			return index;

		index++;
	}

	return -1;
}

MoSkeletonBone * MoSkeleton::GetSkeletonBone(int index)
{
	return skeletonBones[index].second;
}

D3DXMATRIX MoSkeleton::GetBoneAnimationTransforms(wstring boneName)
{
	UINT index = -1;
	for (size_t i = 0; i < skeletonBones.size(); i++)
	{
		if (boneName == skeletonBones[i].first)
		{
			index = i;
			break;
		}
	}
	if(index != -1)
		return boneAnimationTransforms[index];
	else
		return D3DXMATRIX();
}

void MoSkeleton::UpdateAnimation(MoAnimationController * animationController)
{
	MoAnimation* currentAnimation = animationController->GetCurrentAnimation();
	int keyFrame = animationController->GetCurrentKeyFrame();

	if (currentAnimation == NULL)
		return;

	BuildBoneTransforms(animationController, currentAnimation->GetName());
}

void MoSkeleton::Write(BinaryWriter * w)
{
	w->UInt(boneCount);

	for (UINT i = 0; i < boneCount; i++)
	{
		w->WString(skeletonBones[i].first);
		skeletonBones[i].second->Write(w);
	}
}

void MoSkeleton::Read(BinaryReader * r, ModelSkeleton * modelSkeleton)
{
	modelSkeleton->boneCount = r->UInt();
	modelSkeleton->skeletonBones.resize(modelSkeleton->boneCount);
	for (int i = 0; i < modelSkeleton->boneCount; i++)
	{
		modelSkeleton->skeletonBones[i].first = r->Wstring();
		modelSkeleton->skeletonBones[i].second = new ModelSkeletonBone();
		MoSkeletonBone::Read(r, modelSkeleton->skeletonBones[i].second);
	}
}

void MoSkeleton::Write(BinaryWriter * w, ModelSkeleton * modelSkeleton)
{
	w->UInt(modelSkeleton->GetBoneCount());
	for (int i = 0; i < modelSkeleton->GetBoneCount(); i++)
	{
		w->WString(modelSkeleton->skeletonBones[i].first);
		MoSkeletonBone::Write(w, modelSkeleton->skeletonBones[i].second);
	}
}

void MoSkeleton::BuildBoneTransforms(MoAnimationController * animationController, wstring animationName)
{
	if (skinTransforms == NULL)
		skinTransforms = new D3DXMATRIX[boneCount];

	if (boneAnimationTransforms == NULL)	
		boneAnimationTransforms = new D3DXMATRIX[boneCount];


	int keyFrame = animationController->GetCurrentKeyFrame();
	int nextKeyFrame = animationController->GetNextKeyFrame();
	float keyframeFactor = animationController->GetKeyFrameFactor();
	bool useQuaterniokeyFrames = animationController->UseQuaternionKeyFrames();

	int index = 0;
	vector<Pair>::const_iterator it = skeletonBones.begin();
	for (; it != skeletonBones.end(); ++it)
	{
		MoSkeletonBone* bone = it->second;
		
		D3DXMATRIX matInvBindPose = bone->GetInvBindPoseTransform();
		D3DXMATRIX matBoneReference = bone->GetBoneReferenceTransform();
		D3DXMATRIX matAnimation;
		D3DXMATRIX matParentAnimation;

		MoAnimationKeyFrames* animatiokeyFrames = bone->GetAnimationKeyFrames(animationName);

		if (useQuaterniokeyFrames == true)
		{
			const MoAnimationQuaternionKeyFrame& qKeyFrame1 = animatiokeyFrames->GetQuaternionKeyFrame(keyFrame);
			const MoAnimationQuaternionKeyFrame& qKeyFrame2 = animatiokeyFrames->GetQuaternionKeyFrame(nextKeyFrame);
			const D3DXQUATERNION& q1 = qKeyFrame1.GetQuaternion();
			const D3DXQUATERNION& q2 = qKeyFrame2.GetQuaternion();

			D3DXQUATERNION q;

			D3DXQuaternionSlerp(&q, &q1, &q2, keyframeFactor);
			D3DXMatrixRotationQuaternion(&matAnimation, &q);

			D3DXVECTOR3 t1 = qKeyFrame1.GetTranslation();
			D3DXVECTOR3 t2 = qKeyFrame2.GetTranslation();
			
			
			D3DXVECTOR3 vTranslation;
			D3DXVec3Lerp(&vTranslation, &t1, &t2, keyframeFactor);

			matAnimation._41 = vTranslation.x;
			matAnimation._42 = vTranslation.y;
			matAnimation._43 = vTranslation.z;
		}
		else
		{
			matAnimation = animatiokeyFrames->GetKeyFrameTransform(keyFrame);
		}


		int parentBoneIndex = bone->GetParentBoneIndex();

		if (parentBoneIndex < 0)
		{
			D3DXMatrixIdentity(&matParentAnimation);
			matAnimation = bone->GetBindPoseTransform();
			if(animationController->GetCurrentAnimation()->GetUseRootMotion() == true)
				matAnimation = animatiokeyFrames->GetKeyFrameTransform(0);
		}
		else
			matParentAnimation = boneAnimationTransforms[parentBoneIndex];

		boneAnimationTransforms[index] = matAnimation * matParentAnimation;
		skinTransforms[index] = matBoneReference * matInvBindPose *  boneAnimationTransforms[index];

		
		index++;
	}
	int a = 10;
}
