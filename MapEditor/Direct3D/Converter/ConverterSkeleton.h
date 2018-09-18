#pragma once
#include "../Utility/BinaryInputOutputHandler.h"
#include "ConverterEnums.h"

/********************************************************************************
 @brief
 ��� Skeleton Bone�� ������ ����Ͽ� �����ϴ� Class

 �� Bone�� (Name, Index)�� �����Ͽ� �����

********************************************************************************/

class ConverterSkeletonBone;
class ConverterAnimationController;
class ConverterSkeleton : public BinaryInputOutputHandler
{
public:
	ConverterSkeleton();
	ConverterSkeleton(const ConverterSkeleton& otherSkeleton);
	~ConverterSkeleton();

	void AddSkeletonBone(ConverterSkeletonBone* skeletonBone);
	ConverterSkeletonBone* FindBone(wstring boneName);
	int FindBoneIndex(wstring boneName);

	D3DXMATRIX& GetBoneAnimationTransform(int boneIdx);

	ConverterSkeletonBone* GetSkeletonBone(int index);
	const vector<pair<wstring, ConverterSkeletonBone *>>& GetSkeletonBones() { return skeletonBones; }
	
	D3DXMATRIX* GetSkinTransforms() { return skinTransforms; }

	int GetBoneCount() { return boneCount; }

	void AddAnimationFromOtherSkeleton(wstring srcAnimName, wstring dstAnimName, ConverterSkeleton& srcSkeleton);

	void UpdateAnimation(ConverterAnimationController* animationController); ///�� �����Ӹ��� ��� ���� Transform ������ ����

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	void BuildBoneTransforms(ConverterAnimationController* animationController, wstring animationName, RootType rootType);

	int boneCount;

	typedef pair<wstring, ConverterSkeletonBone *> Pair;
	vector<Pair> skeletonBones;

	D3DXMATRIX* skinTransforms; /// ���������� Vertex�� ����� Transform Matrix �迭
	D3DXMATRIX* boneAnimationTransforms; /// �� Bone�� Transform Matrix �迭
	 
};
