#pragma once

class ModelAnimationKeyFrames;

//���� ���� (�ִϸ��̼��� �� ���븦 �����δ�.)
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

	D3DXMATRIX matBindPoseTransform; //Bone�� �ʱ� ���� ��ġ��
	D3DXMATRIX matInvBindPoseTransform; //�ʱ� ���� �����
	D3DXMATRIX matBoneReferenceTransform; 
	D3DXMATRIX matInvBoneReferenceTransform;

	//�θ� ������ �ε�����ȣ.
	int parentBoneIndex;
};