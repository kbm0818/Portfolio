#pragma once

class ModelSkeletonBone;
class ModelAnimationController;
class ModelSkeleton
{
public:
	ModelSkeleton();
	~ModelSkeleton();

	void AddSkeletonBone(ModelSkeletonBone* skeletonBone);
	ModelSkeletonBone* FindBone(wstring boneName);
	int FindBoneIndex(wstring boneName);

	ModelSkeletonBone* GetSkeletonBone(int index);
	const vector<pair<wstring, ModelSkeletonBone *>>& GetSkeletonBones() 
	{ 
		return skeletonBones; 
	}
	
	D3DXMATRIX* GetSkinTransforms() 
	{ 
		return skinTransforms; 
	}
	
	D3DXMATRIX GetBoneAnimationTransforms(wstring boneName);


	int GetBoneCount() { return boneCount; }

	void UpdateAnimation(ModelAnimationController* animationController);

	void DeleteAnimation(UINT index);

private:
	friend class MoSkeleton;

	void BuildBoneTransforms(ModelAnimationController* animationController, wstring animationName);

	typedef pair<wstring, ModelSkeletonBone *> Pair;
	vector<Pair> skeletonBones;

	D3DXMATRIX* skinTransforms;
	D3DXMATRIX* boneAnimationTransforms;
	 
	int boneCount;
};