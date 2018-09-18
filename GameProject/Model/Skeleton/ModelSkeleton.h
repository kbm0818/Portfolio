#pragma once

class ModelSkeletonBone;
class ModelAnimationController;

//������ �Ѱ��ؼ� �����ϴ� Ŭ����.
class ModelSkeleton
{
public:
	ModelSkeleton();
	~ModelSkeleton();
	ModelSkeleton(ModelSkeleton& copy);
	void CopyOtherSkeleton(ModelSkeleton* skeleton);

	//���ο� ���� �߰��Ѵ�.
	void AddSkeletonBone(ModelSkeletonBone* skeletonBone);
	//�̸����� Bone�� ã�´�.
	ModelSkeletonBone* FindBone(wstring boneName);
	//�ش� �̸��� Bone�� �ε����� ���Ѵ�.
	int FindBoneIndex(wstring boneName);

	//�ش� �ε����� Bone�� �����´�.
	ModelSkeletonBone* GetSkeletonBone(int index);
	//���̷��� �ȿ� ����ִ� ������ ����Ʈ�� �����´�.
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

	//Skeleton �ȿ� �����ϴ� Bone���� �̸��� �Բ� ����.
	typedef pair<wstring, ModelSkeletonBone*> Pair;
	vector<Pair> skeletonBones;

	//Transform ����� �迭
	D3DXMATRIX* skinTransforms;
	D3DXMATRIX* boneAnimationTransforms;

	int boneCount;

};