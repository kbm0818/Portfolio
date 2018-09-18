#include "stdafx.h"
#include "ModelBuffer.h"
#include "ModelSkeleton.h"
#include "ModelSkeletonBone.h"
#include "ModelAnimation.h"
#include "ModelAnimationController.h"
#include "ModelAnimationKeyFrames.h"

ModelSkeleton::ModelSkeleton()
{
	skeletonBuffer = new SkeletonBuffer();

	skinTransforms = NULL;
	boneAnimationTransforms = NULL;

	boneCount = 0;
}

ModelSkeleton::~ModelSkeleton()
{
	for each(Pair temp in skeletonBones)
		SAFE_DELETE(temp.second);

	SAFE_DELETE_ARRAY(boneAnimationTransforms);
	SAFE_DELETE_ARRAY(skinTransforms);
}

void ModelSkeleton::Load(BinaryReader * r, GlobalValues* values)
{
	boneCount = r->UInt();
	skeletonBones.resize(boneCount);
	for (UINT i = 0; i < boneCount; i++)
	{
		skeletonBones[i].first = r->Wstring();
		skeletonBones[i].second = new ModelSkeletonBone;
		skeletonBones[i].second->Load(r, values);
	}
}

void ModelSkeleton::AddSkeletonBone(ModelSkeletonBone * skeletonBone)
{
	skeletonBones.push_back(Pair(skeletonBone->GetName(), skeletonBone));
	
	boneCount++;
}

ModelSkeletonBone * ModelSkeleton::FindBone(wstring boneName)
{
	for each(Pair temp in skeletonBones)
	{
		if (boneName == temp.first)
			return temp.second;
	}

	vector<Pair>::iterator iter;
	for (iter = skeletonBones.end(); iter != skeletonBones.begin(); iter--)
	{
		if (String::Contain(boneName, iter->first))
		{
			return iter->second;
		}
	}
	
	return NULL;
}

int ModelSkeleton::FindBoneIndex(wstring boneName)
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

ModelSkeletonBone * ModelSkeleton::GetSkeletonBone(int index)
{
	return skeletonBones[index].second;
}

D3DXMATRIX ModelSkeleton::GetBoneAnimationTransforms(wstring boneName)
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
	if (index != -1 && boneAnimationTransforms != nullptr)
		return boneAnimationTransforms[index];
	else
		return FindBone(boneName)->GetBindPoseTransform();
}

void ModelSkeleton::UpdateAnimation(ModelAnimationController * animationController)
{
	ModelAnimation* currentAnimation = animationController->GetCurrentAnimation();
	int keyFrame = animationController->GetCurrentKeyFrame();

	if (currentAnimation == NULL)
		return;

	BuildBoneTransforms(animationController, currentAnimation->GetName());
}

void ModelSkeleton::DeleteAnimation(UINT index)
{
	for each(Pair temp in skeletonBones)
	{
		temp.second->DeleteAnimation(index);
	}
}

void ModelSkeleton::BuildBoneTransforms(ModelAnimationController * animationController, wstring animationName)
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
		ModelSkeletonBone* bone = it->second;

		D3DXMATRIX matInvBindPose = bone->GetInvBindPoseTransform();
		D3DXMATRIX matBoneReference = bone->GetBoneReferenceTransform();
		D3DXMATRIX matAnimation;
		D3DXMATRIX matParentAnimation;

		ModelAnimationKeyFrames* animatiokeyFrames = bone->GetAnimationKeyFrames(animationName);

		if (useQuaterniokeyFrames == true)
		{
			const ModelAnimationQuaternionKeyFrame& qKeyFrame1 = animatiokeyFrames->GetQuaternionKeyFrame(keyFrame);
			const ModelAnimationQuaternionKeyFrame& qKeyFrame2 = animatiokeyFrames->GetQuaternionKeyFrame(nextKeyFrame);
			const D3DXQUATERNION& q1 = qKeyFrame1.GetQuaternion();
			const D3DXQUATERNION& q2 = qKeyFrame2.GetQuaternion();
			const D3DXQUATERNION& minus_q2 = -q2;

			D3DXQUATERNION q;

			//D3DXQuaternionSlerp(&q, &q1, &q2, keyframeFactor);
			if (D3DXQuaternionLengthSq(&(q1 - q2)) > D3DXQuaternionLengthSq(&(q1 + q2)))
				D3DXQuaternionSlerp(&q, &q1, &minus_q2, keyframeFactor); // Quaternion(회전축, 회전각) 선형 구면 보간
			else
				D3DXQuaternionSlerp(&q, &q1, &q2, keyframeFactor); // Quaternion(회전축, 회전각) 선형 구면 보간

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
			
			if (animationController->GetCurrentAnimation()->GetUseRootMotion() == true)
				matAnimation = animatiokeyFrames->GetKeyFrameTransform(0);
			else
				matAnimation *= bone->GetBindPoseTransform();
		}
		else
			matParentAnimation = boneAnimationTransforms[parentBoneIndex];

		boneAnimationTransforms[index] = matAnimation * matParentAnimation;
		skinTransforms[index] = matBoneReference * matInvBindPose * boneAnimationTransforms[index];

		index++;
	}
}
