#pragma once

class ModelAnimationKeyFrames;

//뼈대 정보 (애니메이션은 이 뼈대를 움직인다.)
class ModelSkeletonBone
{
public:
	ModelSkeletonBone(wstring name, int parentBoneIndex);
	~ModelSkeletonBone();
	ModelSkeletonBone(ModelSkeletonBone& copy);
	ModelSkeletonBone(ModelSkeletonBone&& move);

	void SetBindPoseTransform(const D3DXMATRIX& matBindPoseTransform);
	void SetBoneReferenceTransform(const D3DXMATRIX& matBoneReferenceTransform);

	const D3DXMATRIX& GetBindPoseTransform()
	{
		return matBindPoseTransform;
	}
	const D3DXMATRIX& GetInvBindPoseTransform()
	{
		return matInvBindPoseTransform;
	}
	const D3DXMATRIX& GetBoneReferenceTransform()
	{
		return matBoneReferenceTransform;
	}
	const D3DXMATRIX& GetInvBoneReferenceTransform()
	{
		return matInvBoneReferenceTransform;
	}

	wstring GetName() { return name; }
	void SetName(wstring name) { this->name = name; }
	int GetParentBoneIndex() { return parentBoneIndex; }
	void SetParentBoneIndex(int set) { parentBoneIndex = set; }

private:
	friend class ModelLoader;

	typedef pair<wstring, ModelAnimationKeyFrames*> Pair;

	wstring name;

	D3DXMATRIX matBindPoseTransform; //Bone의 초기 포즈 위치값
	D3DXMATRIX matInvBindPoseTransform; //초기 포즈 역행렬
	D3DXMATRIX matBoneReferenceTransform; 
	D3DXMATRIX matInvBoneReferenceTransform;

	//부모 뼈대의 인덱스번호.
	int parentBoneIndex;
};