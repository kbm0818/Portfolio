#include "../stdafx.h"
#include "ConverterSkeleton.h"
#include "ConverterSkeletonBone.h"
#include "ConverterAnimation.h"
#include "ConverterAnimationController.h"
#include "ConverterAnimationKeyFrames.h"

ConverterSkeleton::ConverterSkeleton()
{
	skinTransforms = NULL;
	boneAnimationTransforms = NULL;

	boneCount = 0;
}

ConverterSkeleton::ConverterSkeleton(const ConverterSkeleton & otherSkeleton)
{
	boneCount = otherSkeleton.boneCount;

	skinTransforms = NULL;
	boneAnimationTransforms = NULL;
	/*if (otherSkeleton.skinTransforms != NULL)
	{
		skinTransforms = new D3DXMATRIX[boneCount];
		memcpy(skinTransforms, otherSkeleton.skinTransforms, sizeof(D3DXMATRIX) * boneCount);
	}
	if (otherSkeleton.boneAnimationTransforms != NULL)
	{
		boneAnimationTransforms = new D3DXMATRIX[boneCount];
		memcpy(boneAnimationTransforms, otherSkeleton.boneAnimationTransforms, sizeof(D3DXMATRIX) * boneCount);
	}
*/
	size_t skeletonBoneCount = (size_t)boneCount;
	skeletonBones.reserve(skeletonBoneCount);
	for (size_t i = 0; i < skeletonBoneCount; i++)
	{
		wstring tempName = otherSkeleton.skeletonBones[i].first;
		ConverterSkeletonBone tempSkeletonBone = *(otherSkeleton.skeletonBones[i].second);

		skeletonBones.push_back({ tempName, new ConverterSkeletonBone(tempSkeletonBone) });
	}
}

ConverterSkeleton::~ConverterSkeleton()
{
	for each(Pair temp in skeletonBones)
		SAFE_DELETE(temp.second);

	SAFE_DELETE_ARRAY(boneAnimationTransforms);
	SAFE_DELETE_ARRAY(skinTransforms);
}

void ConverterSkeleton::AddSkeletonBone(ConverterSkeletonBone * skeletonBone)
{
	skeletonBones.push_back(Pair(skeletonBone->GetName(), skeletonBone));
	
	boneCount++;
}

ConverterSkeletonBone * ConverterSkeleton::FindBone(wstring boneName)
{
	for each(Pair temp in skeletonBones)
	{
		if (boneName == temp.first)
			return temp.second;
	}
	
	return NULL;
}

int ConverterSkeleton::FindBoneIndex(wstring boneName)
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

D3DXMATRIX & ConverterSkeleton::GetBoneAnimationTransform(int boneIdx)
{
	return boneAnimationTransforms[boneIdx];
}

ConverterSkeletonBone * ConverterSkeleton::GetSkeletonBone(int index)
{
	return skeletonBones[index].second;
}

void ConverterSkeleton::AddAnimationFromOtherSkeleton(wstring srcAnimName, wstring dstAnimName, ConverterSkeleton & srcSkeleton)
{
	vector<Pair> srcSkeletonBones = srcSkeleton.skeletonBones;

	vector<Pair>::const_iterator it = srcSkeletonBones.begin();
	for (; it != srcSkeletonBones.end(); ++it)
	{
		ConverterSkeletonBone* srcBone = it->second;
		ConverterSkeletonBone* dstBone = NULL;

		wstring srcBoneName = it->first;
		dstBone = FindBone(srcBoneName);

		if (dstBone != NULL)
		{
			ConverterAnimationKeyFrames* srcAnimKeyFrames = srcBone->GetAnimationKeyFrames(srcAnimName);
			ConverterAnimationKeyFrames* dstAnimKeyFrames = new ConverterAnimationKeyFrames(*srcAnimKeyFrames);
			dstAnimKeyFrames->SetAnimationName(dstAnimName);
			dstBone->AddAnimationKeyFrames(dstAnimKeyFrames);
		}
	}
}

/****************************************************************************************************
 @brief
 ���� Animation�� Final Transform ������ ������Ʈ
****************************************************************************************************/
void ConverterSkeleton::UpdateAnimation(ConverterAnimationController * animationController)
{
	ConverterAnimation* currentAnimation = animationController->GetCurrentAnimation();
	int keyFrame = animationController->GetCurrentKeyFrame();

	if (currentAnimation == NULL)
		return;

	RootType rootType = currentAnimation->GetRootType();
	BuildBoneTransforms(animationController, currentAnimation->GetName(), rootType);
}

void ConverterSkeleton::Export(BinaryWriter * bw)
{
	bw->Write(boneCount);

	UINT skeletonBonesCount = skeletonBones.size();
	bw->Write(skeletonBonesCount);
	for (size_t i = 0; i < skeletonBonesCount; i++)
	{
		bw->Write(skeletonBones[i].first);
		(skeletonBones[i].second)->Export(bw);
	}

	bw->Write(skinTransforms, sizeof(D3DXMATRIX), boneCount);
	bw->Write(boneAnimationTransforms, sizeof(D3DXMATRIX), boneCount);
}

void ConverterSkeleton::Import(BinaryReader * br)
{
	boneCount = br->Int();

	UINT skeletonBonesCount = br->UInt();
	skeletonBones.clear();
	skeletonBones.reserve(skeletonBonesCount);
	for (size_t i = 0; i < skeletonBonesCount; i++)
	{
		wstring name = br->WString();
		ConverterSkeletonBone* ConverterSkeletonBone = new ConverterSkeletonBone();
		ConverterSkeletonBone->Import(br);
		skeletonBones.push_back(make_pair(name, ConverterSkeletonBone));
	}

	SAFE_DELETE_ARRAY(skinTransforms);
	skinTransforms = new D3DXMATRIX[boneCount];
	br->Read(skinTransforms, sizeof(D3DXMATRIX), boneCount);

	SAFE_DELETE_ARRAY(boneAnimationTransforms);
	boneAnimationTransforms = new D3DXMATRIX[boneCount];
	br->Read(boneAnimationTransforms, sizeof(D3DXMATRIX), boneCount);
}

/****************************************************************************************************
 @brief
 �ش� Animation�� ���� Time������ KeyFrame�� ����(���� KeyFrame, ���� KeyFrmae, KeyFrame Factor)�� ���� 
 �� Bone�� ������ Tranform�� ����Ͽ�, �׷κ��� FinalTransform �� ����
****************************************************************************************************/
void ConverterSkeleton::BuildBoneTransforms(ConverterAnimationController* animationController, wstring animationName, RootType rootType)
{
	if (skinTransforms == NULL)
		skinTransforms = new D3DXMATRIX[boneCount];

	if (boneAnimationTransforms == NULL)	
		boneAnimationTransforms = new D3DXMATRIX[boneCount];


	int keyFrame = animationController->GetCurrentKeyFrame();
	int nextKeyFrame = animationController->GetNextKeyFrame();
	float keyframeFactor = animationController->GetKeyFrameFactor();
	bool useQuaterniokeyFrames = animationController->UseQuaternionKeyFrames(); // ���� ���� ����

	int index = 0;
	vector<Pair>::const_iterator it = skeletonBones.begin();
	for (; it != skeletonBones.end(); ++it)
	{
		ConverterSkeletonBone* bone = it->second;
				
		D3DXMATRIX matInvBindPose = bone->GetInvBindPoseTransform(); // ��Ʈ->��
		D3DXMATRIX matBoneReference = bone->GetBoneReferenceTransform(); // ???
		D3DXMATRIX matAnimation; // Local Transform(�θ� ��->�ڽ� ��)�� ������ Transform
		D3DXMATRIX matParentAnimation; // �θ� ���� Local Transform(�θ��� �θ� ��->�θ� ��)�� ������ Transform

		D3DXMATRIX matFirstRootTransform;

		ConverterAnimationKeyFrames* animatiokeyFrames = bone->GetAnimationKeyFrames(animationName);

		if (useQuaterniokeyFrames == true)
		{
			// ���� KeyFrame�� ���� KeyFrame�� Quaternion �� �ε�
			const ConverterAnimationQuaternionKeyFrame& qKeyFrame1 = animatiokeyFrames->GetQuaternionKeyFrame(keyFrame);
			const ConverterAnimationQuaternionKeyFrame& qKeyFrame2 = animatiokeyFrames->GetQuaternionKeyFrame(nextKeyFrame);
			const D3DXQUATERNION& q1 = qKeyFrame1.GetQuaternion();
			const D3DXQUATERNION& q2 = qKeyFrame2.GetQuaternion();
			const D3DXQUATERNION& minus_q2 = -q2;

			D3DXQUATERNION q; // ������ Quaternion

			if(D3DXQuaternionLengthSq(&(q1-q2)) > D3DXQuaternionLengthSq(&(q1+q2)))
				D3DXQuaternionSlerp(&q, &q1, &minus_q2, keyframeFactor); // Quaternion(ȸ����, ȸ����) ���� ���� ����
			else
				D3DXQuaternionSlerp(&q, &q1, &q2, keyframeFactor); // Quaternion(ȸ����, ȸ����) ���� ���� ����

			D3DXMatrixRotationQuaternion(&matAnimation, &q); // Quaternion�� Trnsform Matrix�� ��ȯ

			D3DXVECTOR3 t1 = qKeyFrame1.GetTranslation();
			D3DXVECTOR3 t2 = qKeyFrame2.GetTranslation();
			
			
			D3DXVECTOR3 vTranslation; // ������ ���� ��ġ
			D3DXVec3Lerp(&vTranslation, &t1, &t2, keyframeFactor); // ��ġ�� ���� ����

			// ȸ�� ��, ȸ������ ���麸���� Matrix�� ���� ������ ��ġ�� �߰�
			matAnimation._41 = vTranslation.x;
			matAnimation._42 = vTranslation.y;
			matAnimation._43 = vTranslation.z;
		}
		else
		{
			// ���� ���� ������� �׳� KeyFrmae Trnsform�� ���
			matAnimation = animatiokeyFrames->GetKeyFrameTransform(keyFrame);
		}


		int parentBoneIndex = bone->GetParentBoneIndex();

		if (parentBoneIndex < 0)
		{
			// ��Ʈ�� ��� �θ� �� TrsnformAnimation�� Idenetity Transform���� ����
			D3DXMatrixIdentity(&matParentAnimation);

			// ��Ʈ ����� ù��° ������ ������ ����
			if (rootType == RootType::RootType_FirstFrame) matAnimation = animatiokeyFrames->GetKeyFrameTransform(0);
			// ��Ʈ ����� �������� ����
			else if (rootType == RootType::RootType_Fixed) matAnimation = bone->GetBindPoseTransform();
		}
		else
			matParentAnimation = boneAnimationTransforms[parentBoneIndex];

		// boneAnimationTransforms = (�ڽ� ��->�θ� ��) * (�θ� ��-�θ��� >�θ� ��) * ... (�θ��� ������...�θ� ��->��Ʈ ��) = (�ڽ� ��->��Ʈ��)
		boneAnimationTransforms[index] = matAnimation * matParentAnimation;

		// Mesh���� Vertex�� Root ������ ���� �ϹǷ�, matInvBindPose(Bone�� �ʱ� ��->��Ʈ)�� ���� ��
		// boneAnimationTransforms(��Ʈ ��-> �ڽ� ��) ���� Animation �� Bone�� ��ġ�� ����
		skinTransforms[index] = matInvBindPose *  boneAnimationTransforms[index];

		index++;
	}
}
