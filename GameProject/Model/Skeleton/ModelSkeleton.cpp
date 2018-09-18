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
	//현재 애니메이션과 keyFrame 인덱스를 가져온다.
	ModelAnimation* currentAnimation = animationController->GetCurrentAnimation();
	int keyFrame = animationController->GetCurrentKeyFrame();

	if (currentAnimation == NULL)
		return;

	//애니메이션 보간과 현재 상태에서 각 Bone들의 최종 Transform정보를 업데이트한다.
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
	bool useQuaterniokeyFrames = animationController->UseQuaternionKeyFrames(); //쿼터니언 보간을 사용할지 말지?

	ModelAnimation* animationClip = animationController->GetCurrentAnimation();

	int index = 0;
	vector<Pair>::const_iterator it = skeletonBones.begin();
	//모든 본들에 대해서 작업을 수행한다. (본들은 부모 -> 자식 순으로 자례대로 벡터에 넣었기 때문에 순차대로 진행된다.)
	for (; it != skeletonBones.end(); ++it)
	{
		ModelSkeletonBone* bone = it->second;

		//각각의 Bone들은 자기의 Local좌표계를 가지고 있다.
		//이 Local좌표계는 부모 Bone에게 영향을 받는다.
		//즉, 부모 Bone들의 Transform정보들을 끝까지 다 타고가야지 정확한 Transform을 구할 수 있다.

		D3DXMATRIX matInvBindPose = bone->GetInvBindPoseTransform(); //BindPose란, T포즈 상태의 위치정보다.
		D3DXMATRIX matBoneReference = bone->GetBoneReferenceTransform();

		//자기 자신과 자기 부모의 애니메이션 매트릭스 (부모의 Transform에 영향 받기 때문.)
		D3DXMATRIX matAnimation;
		D3DXMATRIX matParentAnimation;

		D3DXMATRIX matFirstRootTransform;

		ModelAnimationKeyFrames* animatiokeyFrames = animationClip->GetAnimationKeyFrames(index);//bone->GetAnimationKeyFrames(animationName);

		//사원수 보간
		if (useQuaterniokeyFrames == true)
		{
			//현재 KeyFrame과 다음 KeyFrame의 쿼터니언 값을 로드해온다.
			const ModelAnimationQuaternionKeyFrame& qKeyFrame1 = animatiokeyFrames->GetQuaternionKeyFrame(keyFrame);
			const ModelAnimationQuaternionKeyFrame& qKeyFrame2 = animatiokeyFrames->GetQuaternionKeyFrame(nextKeyFrame);
			const D3DXQUATERNION& q1 = qKeyFrame1.GetQuaternion();
			const D3DXQUATERNION& q2 = qKeyFrame2.GetQuaternion();
			const D3DXQUATERNION& minus_q2 = -q2;

			D3DXQUATERNION q;

			//DX내 Quaternion에 보간해주는 함수가 있는데 그것을 사용한다.
			//Slerp는 선형보간이 아니라 구형보간이다.
			if (D3DXQuaternionLengthSq(&(q1 - q2)) > D3DXQuaternionLengthSq(&(q1 + q2)))
				D3DXQuaternionSlerp(&q, &q1, &minus_q2, keyframeFactor);
			else
				D3DXQuaternionSlerp(&q, &q1, &q2, keyframeFactor);

			//보간된 쿼터니언을 다시 MATRIX로 변환한다.
			D3DXMatrixRotationQuaternion(&matAnimation, &q);

			//프레임 별 위치정보도 가져온다.
			D3DXVECTOR3 t1 = qKeyFrame1.GetTranslation();
			D3DXVECTOR3 t2 = qKeyFrame2.GetTranslation();

			//위치정보는 선형보간을 해준다. 
			//회전정보도 위치정보처럼 선형보간을하면 처음과 끝은 맞겠지만, 중간에 싱크가 안맞게된다.
			D3DXVECTOR3 vTranslation;
			D3DXVec3Lerp(&vTranslation, &t1, &t2, keyframeFactor);

			//회전정보에 위치정보를 추가한다.
			matAnimation._41 = vTranslation.x;
			matAnimation._42 = vTranslation.y;
			matAnimation._43 = vTranslation.z;
		}
		else
		{
			//보간하지 않을경우 그냥 기본 keyFrame 데이터를 사용.
			matAnimation = animatiokeyFrames->GetKeyFrameTransform(keyFrame);
		}


		//여기까지 구한 MATRIX정보는 해당 Bone의 Local좌표계에서의 MATRIX다.
		//최종적인 World좌표계에서의 MATRIX를 구하려면 부모 Bone의 MATRIX값과 곱해주어야한다.
		//그래서 부모의 Index를 가져온다.
		int parentBoneIndex = bone->GetParentBoneIndex();

		//부모의 Index가 -1인 경우 RootBone이므로 부모의 MATRIX는 그냥 Identity로 초기화한다.
		if (parentBoneIndex < 0)
		{

			D3DXMatrixIdentity(&matParentAnimation);

			// 루트 모션을 첫번째 프레임 값으로 고정
			//matAnimation = animatiokeyFrames->GetKeyFrameTransform(0);

			// 루트 모션을 변경하지 않음
			//matAnimation = bone->GetBindPoseTransform();
		}
		else
			//부모의 AnimationTransform을 가져온다. (계층구조로 Root부터 차례대로 내려왔기 때문에 문제가 없다.)
			matParentAnimation = boneAnimationTransforms[parentBoneIndex];

		//boneAnimationTransforms = 자신 본 * 부모 본 
		//이 때의 ParentAnimation은 World좌표계로 변환이 완료된 최종 값이다. 즉, 부모의 부모 ~ Root Bone까지의 모든 변환이 누적되어 적용된 상태.
		boneAnimationTransforms[index] = matAnimation * matParentAnimation;

		//최종적으로 쉐이더에 넘겨줄 Transform 행렬은 초기 T포즈 상태의 MATRIX와 곱해주어야한다.
		//Mesh상의 Vertex는 각 Mesh의 로컬 좌표계에서의 위치정보를 가지고 있으므로, 
		//먼저 matInvBindPose를 곱해서 초기 포즈로 변환해준 뒤에
		//애니메이션된 정보로 또 변환을 해주는 것이다. 이 2가지 작업을 한번에 묶어서한다.
		skinTransforms[index] = matInvBindPose * boneAnimationTransforms[index];

		index++;
	}
}
