#pragma once

class ModelSkeletonBone;
class ModelAnimationController;

//본들을 총괄해서 관리하는 클래스.
class ModelSkeleton
{
public:
	ModelSkeleton();
	~ModelSkeleton();
	ModelSkeleton(ModelSkeleton& copy);
	void CopyOtherSkeleton(ModelSkeleton* skeleton);

	//새로운 본을 추가한다.
	void AddSkeletonBone(ModelSkeletonBone* skeletonBone);
	//이름으로 Bone을 찾는다.
	ModelSkeletonBone* FindBone(wstring boneName);
	//해당 이름의 Bone의 인덱스를 구한다.
	int FindBoneIndex(wstring boneName);

	//해당 인덱스의 Bone을 가져온다.
	ModelSkeletonBone* GetSkeletonBone(int index);
	//스켈레톤 안에 들어있는 본들의 리스트를 가져온다.
	const vector<pair<wstring, ModelSkeletonBone*>>& GetSkeletonBones()
	{
		return skeletonBones;
	}

	D3DXMATRIX* GetSkinTransforms() { return skinTransforms; }
	int GetBoneCount() { return boneCount; }
	D3DXMATRIX& GetBoneTransform(int index) { return boneAnimationTransforms[index]; }

	void DeleteBone(int index);
	void AddBone(int index, ModelSkeletonBone* bone);

	void UpdateAnimation(ModelAnimationController* animationController);
	
	void UpdateBoneCount();
	void ChangeBoneName(int index, wstring name);

private:
	friend class ModelLoader;

	void BuildBoneTransforms(ModelAnimationController* animationController, wstring animationName);

	//Skeleton 안에 존재하는 Bone들을 이름과 함께 저장.
	typedef pair<wstring, ModelSkeletonBone*> Pair;
	vector<Pair> skeletonBones;

	//Transform 행렬의 배열
	D3DXMATRIX* skinTransforms;
	D3DXMATRIX* boneAnimationTransforms;

	int boneCount;

};