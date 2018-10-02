#pragma once

class ModelSkeletonBone;
class ModelAnimationController;
class SkeletonBuffer;
class ModelSkeleton
{
public:
	ModelSkeleton();
	~ModelSkeleton();

	void Load(BinaryReader* r, GlobalValues* values);

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

	SkeletonBuffer* GetBuffer()
	{
		return skeletonBuffer;
	}

	void UpdateAnimation(ModelAnimationController* animationController);

	void DeleteAnimation(UINT index);

private:
	friend class MoSkeleton;

	void BuildBoneTransforms(ModelAnimationController* animationController, wstring animationName);

	typedef pair<wstring, ModelSkeletonBone *> Pair;
	vector<Pair> skeletonBones;

	D3DXMATRIX* skinTransforms;
	D3DXMATRIX* boneAnimationTransforms;
	 
	UINT boneCount;

	SkeletonBuffer* skeletonBuffer;
};
