#pragma once

class MoAnimationKeyFrames;
class ModelSkeletonBone;
class MoSkeletonBone
{
public:
	MoSkeletonBone(wstring name, int parentBoneIndex);
	~MoSkeletonBone();

	void AddAnimationKeyFrames(MoAnimationKeyFrames* keyFrames);

	void SetBindPoseTransform(const D3DXMATRIX& matBindPoseTransform);
	void SetBoneReferenceTransform(const D3DXMATRIX& matBoneReferenceTransform);

	MoAnimationKeyFrames* GetAnimationKeyFrames(wstring animationName);

	void Write(BinaryWriter * w);
	static void Write(BinaryWriter * w, ModelSkeletonBone* skelteonBone);
	static void Read(BinaryReader * r, ModelSkeletonBone* skelteonBone);

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
	int GetParentBoneIndex() { return parentBoneIndex; }

private:
	typedef pair<wstring, MoAnimationKeyFrames *> Pair;

	wstring name;

	D3DXMATRIX matBindPoseTransform;
	D3DXMATRIX matInvBindPoseTransform;
	D3DXMATRIX matBoneReferenceTransform;
	D3DXMATRIX matInvBoneReferenceTransform;

	int parentBoneIndex;

	vector<pair<wstring, MoAnimationKeyFrames *>> animationKeyFrames;
};