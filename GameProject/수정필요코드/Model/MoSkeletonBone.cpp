#include "stdafx.h"
#include "MoSkeletonBone.h"
#include "Object/Model/ModelSkeletonBone.h"
#include "MoAnimationKeyFrames.h"
#include "Object/Model/ModelAnimationKeyFrames.h"

MoSkeletonBone::MoSkeletonBone(wstring name, int parentBoneIndex)
	: name(name), parentBoneIndex(parentBoneIndex)
{
	D3DXMatrixIdentity(&matBindPoseTransform);
	D3DXMatrixIdentity(&matInvBindPoseTransform);
	D3DXMatrixIdentity(&matBoneReferenceTransform);
	D3DXMatrixIdentity(&matInvBoneReferenceTransform);
}

MoSkeletonBone::~MoSkeletonBone()
{
	for each(Pair temp in animationKeyFrames)
		SAFE_DELETE(temp.second);
}

void MoSkeletonBone::AddAnimationKeyFrames(MoAnimationKeyFrames * keyFrames)
{
	animationKeyFrames.push_back(Pair(keyFrames->GetAnimationName(), keyFrames));
}

void MoSkeletonBone::SetBindPoseTransform(const D3DXMATRIX & matBindPoseTransform)
{
	this->matBindPoseTransform = matBindPoseTransform;
	D3DXMatrixInverse(&this->matInvBindPoseTransform, NULL, &this->matBindPoseTransform);
}

void MoSkeletonBone::SetBoneReferenceTransform(const D3DXMATRIX & matBoneReferenceTransform)
{
	this->matBoneReferenceTransform = matBoneReferenceTransform;
	D3DXMatrixInverse(&this->matInvBoneReferenceTransform, NULL, &this->matBoneReferenceTransform);
}

MoAnimationKeyFrames * MoSkeletonBone::GetAnimationKeyFrames(wstring animationName)
{
	for each(Pair temp in animationKeyFrames)
	{
		if (temp.first == animationName)
			return temp.second;
	}

	return NULL;
}

void MoSkeletonBone::Write(BinaryWriter * w)
{
	w->WString(name);

	w->Matrix(matBindPoseTransform);
	w->Matrix(matInvBindPoseTransform);
	w->Matrix(matBoneReferenceTransform);
	w->Matrix(matInvBoneReferenceTransform);

	w->UInt(parentBoneIndex);

	w->UInt(animationKeyFrames.size());
	for (size_t i = 0; i < animationKeyFrames.size(); i++)
	{
		w->WString(animationKeyFrames[i].first);
		animationKeyFrames[i].second->Write(w);
	}

}

void MoSkeletonBone::Write(BinaryWriter * w, ModelSkeletonBone * skelteonBone)
{
	w->WString(skelteonBone->name);

	w->Matrix(skelteonBone->matBindPoseTransform);
	w->Matrix(skelteonBone->matInvBindPoseTransform);
	w->Matrix(skelteonBone->matBoneReferenceTransform);
	w->Matrix(skelteonBone->matInvBoneReferenceTransform);

	w->UInt(skelteonBone->parentBoneIndex);

	w->UInt(skelteonBone->animationKeyFrames.size());
	for (size_t i = 0; i < skelteonBone->animationKeyFrames.size(); i++)
	{
		w->WString(skelteonBone->animationKeyFrames[i].first);
		MoAnimationKeyFrames::Write(w, skelteonBone->animationKeyFrames[i].second);
	}
}

void MoSkeletonBone::Read(BinaryReader * r, ModelSkeletonBone * skelteonBone)
{
	skelteonBone->name = r->Wstring();

	skelteonBone->matBindPoseTransform = r->Matrix();
	skelteonBone->matInvBindPoseTransform = r->Matrix();
	skelteonBone->matBoneReferenceTransform = r->Matrix();
	skelteonBone->matInvBoneReferenceTransform = r->Matrix();

	skelteonBone->parentBoneIndex = r->UInt();

	UINT size = r->UInt();
	skelteonBone->animationKeyFrames.resize(size);
	for (UINT i = 0; i < size; i++)
	{
		skelteonBone->animationKeyFrames[i].first = r->Wstring();
		skelteonBone->animationKeyFrames[i].second = new ModelAnimationKeyFrames();
		MoAnimationKeyFrames::Read(r, skelteonBone->animationKeyFrames[i].second);
	}
}
