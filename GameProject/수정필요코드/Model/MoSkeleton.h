#pragma once

class MoSkeletonBone;
class MoAnimationController;
class ModelSkeleton;
class MoSkeleton
{
public:
	MoSkeleton();
	~MoSkeleton();

	void AddSkeletonBone(MoSkeletonBone* skeletonBone);
	MoSkeletonBone* FindBone(wstring boneName);
	int FindBoneIndex(wstring boneName);

	MoSkeletonBone* GetSkeletonBone(int index);
	const vector<pair<wstring, MoSkeletonBone *>>& GetSkeletonBones() 
	{ 
		return skeletonBones; 
	}
	
	D3DXMATRIX* GetSkinTransforms() 
	{ 
		return skinTransforms; 
	}
	
	D3DXMATRIX GetBoneAnimationTransforms(wstring boneName);


	int GetBoneCount() { return boneCount; }

	void UpdateAnimation(MoAnimationController* animationController);

	void Write(BinaryWriter* w);
	static void Read(BinaryReader* r, ModelSkeleton* modelSkeleton);
	static void Write(BinaryWriter* w, ModelSkeleton* modelSkeleton);

private:
	void BuildBoneTransforms(MoAnimationController* animationController, wstring animationName);

	typedef pair<wstring, MoSkeletonBone *> Pair;
	vector<Pair> skeletonBones;

	D3DXMATRIX* skinTransforms;
	D3DXMATRIX* boneAnimationTransforms;
	 
	UINT boneCount;
};
