#include "stdafx.h"
#include "ModelSkeletonBone.h"
#include "../../Model/Animation/ModelAnimationKeyFrames.h"

ModelSkeletonBone::ModelSkeletonBone(wstring name, int parentBoneIndex)
	: name(name), parentBoneIndex(parentBoneIndex)
{
	D3DXMatrixIdentity(&matBindPoseTransform);
	D3DXMatrixIdentity(&matInvBindPoseTransform);
	D3DXMatrixIdentity(&matBoneReferenceTransform);
	D3DXMatrixIdentity(&matInvBoneReferenceTransform);
}

ModelSkeletonBone::~ModelSkeletonBone()
{
}

ModelSkeletonBone::ModelSkeletonBone(ModelSkeletonBone & copy)
{
	name = copy.name;
	parentBoneIndex = copy.parentBoneIndex;

	memcpy(&matBindPoseTransform, &copy.matBindPoseTransform, sizeof(D3DXMATRIX));
	memcpy(&matInvBindPoseTransform, &copy.matInvBindPoseTransform, sizeof(D3DXMATRIX));
	memcpy(&matBoneReferenceTransform, &copy.matBoneReferenceTransform, sizeof(D3DXMATRIX));
	memcpy(&matInvBoneReferenceTransform, &copy.matInvBoneReferenceTransform, sizeof(D3DXMATRIX));
}

ModelSkeletonBone::ModelSkeletonBone(ModelSkeletonBone && move)
{
	name = move.name;
	parentBoneIndex = move.parentBoneIndex;

	memcpy(&matBindPoseTransform, &move.matBindPoseTransform, sizeof(D3DXMATRIX));
	memcpy(&matInvBindPoseTransform, &move.matInvBindPoseTransform, sizeof(D3DXMATRIX));
	memcpy(&matBoneReferenceTransform, &move.matBoneReferenceTransform, sizeof(D3DXMATRIX));
	memcpy(&matInvBoneReferenceTransform, &move.matInvBoneReferenceTransform, sizeof(D3DXMATRIX));
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
