#include "stdafx.h"
#include "ModelSkeletonBone.h"
#include "ModelAnimationKeyFrames.h"

ModelSkeletonBone::ModelSkeletonBone(wstring name, int parentBoneIndex)
	: name(name), parentBoneIndex(parentBoneIndex)
{
	D3DXMatrixIdentity(&matBindPoseTransform);
	D3DXMatrixIdentity(&matInvBindPoseTransform);
	D3DXMatrixIdentity(&matBoneReferenceTransform);
	D3DXMatrixIdentity(&matInvBoneReferenceTransform);
}

ModelSkeletonBone::ModelSkeletonBone()
{
	D3DXMatrixIdentity(&matBindPoseTransform);
	D3DXMatrixIdentity(&matInvBindPoseTransform);
	D3DXMatrixIdentity(&matBoneReferenceTransform);
	D3DXMatrixIdentity(&matInvBoneReferenceTransform);
}

ModelSkeletonBone::~ModelSkeletonBone()
{
	for each(Pair temp in animationKeyFrames)
		SAFE_DELETE(temp.second);
}

void ModelSkeletonBone::Load(BinaryReader * r, GlobalValues* values)
{
	name = r->Wstring();

	matBindPoseTransform = r->Matrix();
	matInvBindPoseTransform = r->Matrix();
	matBoneReferenceTransform = r->Matrix();
	matInvBoneReferenceTransform = r->Matrix();

	parentBoneIndex = r->UInt();

	UINT count = r->UInt();
	animationKeyFrames.resize(count);
	for (UINT i = 0; i < count; i++)
	{
		animationKeyFrames[i].first = r->Wstring();
		animationKeyFrames[i].second = new ModelAnimationKeyFrames;
		animationKeyFrames[i].second->Load(r);
	}
}

void ModelSkeletonBone::AddAnimationKeyFrames(ModelAnimationKeyFrames * keyFrames)
{
	animationKeyFrames.push_back(Pair(keyFrames->GetAnimationName(), keyFrames));
}

void ModelSkeletonBone::SetBindPoseTransform(const D3DXMATRIX & matBindPoseTransform)
{
	this->matBindPoseTransform = matBindPoseTransform;
	D3DXMatrixInverse(&this->matInvBindPoseTransform, NULL, &this->matBindPoseTransform);
}

void ModelSkeletonBone::SetBoneReferenceTransform(const D3DXMATRIX & matBoneReferenceTransform)
{
	this->matBoneReferenceTransform = matBoneReferenceTransform;
	D3DXMatrixInverse(&this->matInvBoneReferenceTransform, NULL, &this->matBoneReferenceTransform);
}

ModelAnimationKeyFrames * ModelSkeletonBone::GetAnimationKeyFrames(wstring animationName)
{
	for each(Pair temp in animationKeyFrames)
	{
		if (temp.first == animationName)
			return temp.second;
	}

	return NULL;
}

void ModelSkeletonBone::DeleteAnimation(UINT index)
{
	if (index > animationKeyFrames.size())
		return;

	SAFE_DELETE(animationKeyFrames[index].second);
	animationKeyFrames.erase(animationKeyFrames.begin() + index);
}
