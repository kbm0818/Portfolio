#include "stdafx.h"
#include "./ModelSkeleton.h"
#include "./ModelSkeletonBone.h"
#include "../Animation/ModelAnimation.h"
#include "../Animation/ModelAnimationController.h"
#include "../Animation/ModelAnimationKeyFrames.h"

ModelSkeleton::ModelSkeleton()
{
	skinTransforms = NULL;
	boneAnimationTransforms = NULL;
	boneCount = 0;
}

ModelSkeleton::~ModelSkeleton()
{
	for (Pair temp : skeletonBones)
		SAFE_DELETE(temp.second);

	SAFE_DELETE_ARRAY(skinTransforms);
}

ModelSkeleton::ModelSkeleton(ModelSkeleton & copy)
{
	boneCount = copy.boneCount;
	for (int i = 0; i < boneCount; ++i)
	{
		wstring name = copy.skeletonBones[i].first;
		skeletonBones.push_back(make_pair(
			copy.skeletonBones[i].first,
			new ModelSkeletonBone(*copy.skeletonBones[i].second)
			));	
	}
	skinTransforms = nullptr;
	boneAnimationTransforms = nullptr;
}

void ModelSkeleton::CopyOtherSkeleton(ModelSkeleton * skeleton)
{
	SAFE_DELETE_ARRAY(skinTransforms);
	SAFE_DELETE_ARRAY(boneAnimationTransforms);

	for (size_t i = 0; i < skeletonBones.size(); ++i)
		SAFE_DELETE(skeletonBones[i].second);
	skeletonBones.clear();

	for (size_t i = 0; i < skeleton->skeletonBones.size(); ++i)
	{
		skeletonBones.push_back(make_pair(
			skeleton->skeletonBones[i].first,
			skeleton->skeletonBones[i].second
		));
		skeleton->skeletonBones[i].second = nullptr;
	}
	boneCount = skeletonBones.size();
}

void ModelSkeleton::AddSkeletonBone(ModelSkeletonBone * skeletonBone)
{
	skeletonBones.push_back(Pair(skeletonBone->GetName(), skeletonBone));
	boneCount++;
}

ModelSkeletonBone * ModelSkeleton::FindBone(wstring boneName)
{
	for (Pair temp : skeletonBones)
	{
		if (boneName == temp.first)
			return temp.second;
	}
	return NULL;
}

int ModelSkeleton::FindBoneIndex(wstring boneName)
{
	int index = 0;
	for (Pair temp : skeletonBones)
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

void ModelSkeleton::DeleteBone(int index)
{
	SAFE_DELETE(skeletonBones[index].second);
	skeletonBones.erase(skeletonBones.begin() + index);

	UpdateBoneCount();
}

void ModelSkeleton::AddBone(int index, ModelSkeletonBone * bone)
{
	skeletonBones.insert(skeletonBones.begin() + index, make_pair(bone->GetName(), bone));
	UpdateBoneCount();
}

void ModelSkeleton::UpdateAnimation(ModelAnimationController * animationController)
{
	//���� �ִϸ��̼ǰ� keyFrame �ε����� �����´�.
	ModelAnimation* currentAnimation = animationController->GetCurrentAnimation();
	int keyFrame = animationController->GetCurrentKeyFrame();

	if (currentAnimation == NULL)
		return;

	//�ִϸ��̼� ������ ���� ���¿��� �� Bone���� ���� Transform������ ������Ʈ�Ѵ�.
	BuildBoneTransforms(animationController, currentAnimation->GetName());
}

void ModelSkeleton::UpdateBoneCount()
{
	boneCount = (int)skeletonBones.size();
	SAFE_DELETE_ARRAY(skinTransforms);
	SAFE_DELETE_ARRAY(boneAnimationTransforms);
}

void ModelSkeleton::ChangeBoneName(int index, wstring name)
{
	skeletonBones[index].first = name;
	skeletonBones[index].second->SetName(name);
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
	bool useQuaterniokeyFrames = animationController->UseQuaternionKeyFrames(); //���ʹϾ� ������ ������� ����?

	ModelAnimation* animationClip = animationController->GetCurrentAnimation();

	int index = 0;
	vector<Pair>::const_iterator it = skeletonBones.begin();
	//��� ���鿡 ���ؼ� �۾��� �����Ѵ�. (������ �θ� -> �ڽ� ������ �ڷʴ�� ���Ϳ� �־��� ������ ������� ����ȴ�.)
	for (; it != skeletonBones.end(); ++it)
	{
		ModelSkeletonBone* bone = it->second;

		//������ Bone���� �ڱ��� Local��ǥ�踦 ������ �ִ�.
		//�� Local��ǥ��� �θ� Bone���� ������ �޴´�.
		//��, �θ� Bone���� Transform�������� ������ �� Ÿ������ ��Ȯ�� Transform�� ���� �� �ִ�.

		D3DXMATRIX matInvBindPose = bone->GetInvBindPoseTransform(); //BindPose��, T���� ������ ��ġ������.
		D3DXMATRIX matBoneReference = bone->GetBoneReferenceTransform();

		//�ڱ� �ڽŰ� �ڱ� �θ��� �ִϸ��̼� ��Ʈ���� (�θ��� Transform�� ���� �ޱ� ����.)
		D3DXMATRIX matAnimation;
		D3DXMATRIX matParentAnimation;

		D3DXMATRIX matFirstRootTransform;

		ModelAnimationKeyFrames* animatiokeyFrames = animationClip->GetAnimationKeyFrames(index);//bone->GetAnimationKeyFrames(animationName);

		//����� ����
		if (useQuaterniokeyFrames == true)
		{
			//���� KeyFrame�� ���� KeyFrame�� ���ʹϾ� ���� �ε��ؿ´�.
			const ModelAnimationQuaternionKeyFrame& qKeyFrame1 = animatiokeyFrames->GetQuaternionKeyFrame(keyFrame);
			const ModelAnimationQuaternionKeyFrame& qKeyFrame2 = animatiokeyFrames->GetQuaternionKeyFrame(nextKeyFrame);
			const D3DXQUATERNION& q1 = qKeyFrame1.GetQuaternion();
			const D3DXQUATERNION& q2 = qKeyFrame2.GetQuaternion();
			const D3DXQUATERNION& minus_q2 = -q2;

			D3DXQUATERNION q;

			//DX�� Quaternion�� �������ִ� �Լ��� �ִµ� �װ��� ����Ѵ�.
			//Slerp�� ���������� �ƴ϶� ���������̴�.
			if (D3DXQuaternionLengthSq(&(q1 - q2)) > D3DXQuaternionLengthSq(&(q1 + q2)))
				D3DXQuaternionSlerp(&q, &q1, &minus_q2, keyframeFactor);
			else
				D3DXQuaternionSlerp(&q, &q1, &q2, keyframeFactor);

			//������ ���ʹϾ��� �ٽ� MATRIX�� ��ȯ�Ѵ�.
			D3DXMatrixRotationQuaternion(&matAnimation, &q);

			//������ �� ��ġ������ �����´�.
			D3DXVECTOR3 t1 = qKeyFrame1.GetTranslation();
			D3DXVECTOR3 t2 = qKeyFrame2.GetTranslation();

			//��ġ������ ���������� ���ش�. 
			//ȸ�������� ��ġ����ó�� �����������ϸ� ó���� ���� �°�����, �߰��� ��ũ�� �ȸ°Եȴ�.
			D3DXVECTOR3 vTranslation;
			D3DXVec3Lerp(&vTranslation, &t1, &t2, keyframeFactor);

			//ȸ�������� ��ġ������ �߰��Ѵ�.
			matAnimation._41 = vTranslation.x;
			matAnimation._42 = vTranslation.y;
			matAnimation._43 = vTranslation.z;
		}
		else
		{
			//�������� ������� �׳� �⺻ keyFrame �����͸� ���.
			matAnimation = animatiokeyFrames->GetKeyFrameTransform(keyFrame);
		}


		//������� ���� MATRIX������ �ش� Bone�� Local��ǥ�迡���� MATRIX��.
		//�������� World��ǥ�迡���� MATRIX�� ���Ϸ��� �θ� Bone�� MATRIX���� �����־���Ѵ�.
		//�׷��� �θ��� Index�� �����´�.
		int parentBoneIndex = bone->GetParentBoneIndex();

		//�θ��� Index�� -1�� ��� RootBone�̹Ƿ� �θ��� MATRIX�� �׳� Identity�� �ʱ�ȭ�Ѵ�.
		if (parentBoneIndex < 0)
		{

			D3DXMatrixIdentity(&matParentAnimation);

			// ��Ʈ ����� ù��° ������ ������ ����
			//matAnimation = animatiokeyFrames->GetKeyFrameTransform(0);

			// ��Ʈ ����� �������� ����
			//matAnimation = bone->GetBindPoseTransform();
		}
		else
			//�θ��� AnimationTransform�� �����´�. (���������� Root���� ���ʴ�� �����Ա� ������ ������ ����.)
			matParentAnimation = boneAnimationTransforms[parentBoneIndex];

		//boneAnimationTransforms = �ڽ� �� * �θ� �� 
		//�� ���� ParentAnimation�� World��ǥ��� ��ȯ�� �Ϸ�� ���� ���̴�. ��, �θ��� �θ� ~ Root Bone������ ��� ��ȯ�� �����Ǿ� ����� ����.
		boneAnimationTransforms[index] = matAnimation * matParentAnimation;

		//���������� ���̴��� �Ѱ��� Transform ����� �ʱ� T���� ������ MATRIX�� �����־���Ѵ�.
		//Mesh���� Vertex�� �� Mesh�� ���� ��ǥ�迡���� ��ġ������ ������ �����Ƿ�, 
		//���� matInvBindPose�� ���ؼ� �ʱ� ����� ��ȯ���� �ڿ�
		//�ִϸ��̼ǵ� ������ �� ��ȯ�� ���ִ� ���̴�. �� 2���� �۾��� �ѹ��� ����Ѵ�.
		skinTransforms[index] = matInvBindPose * boneAnimationTransforms[index];

		index++;
	}
}
