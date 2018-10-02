#include "stdafx.h"
#include "ModelAnimationKeyFrames.h"

ModelAnimationKeyFrames::ModelAnimationKeyFrames(wstring animationName)
	: animationName(animationName)
{
	
}

ModelAnimationKeyFrames::ModelAnimationKeyFrames()
{
}


ModelAnimationKeyFrames::~ModelAnimationKeyFrames()
{

}

void ModelAnimationKeyFrames::AddKeyFrame(const D3DXMATRIX & transform)
{
	keyFrames.push_back(make_pair(transform, ModelAnimationQuaternionKeyFrame(transform)));
}

wstring ModelAnimationKeyFrames::GetAnimationName()
{
	return animationName;
}

const D3DXMATRIX & ModelAnimationKeyFrames::GetKeyFrameTransform(int keyFrame) const
{
	return keyFrames[keyFrame].first;
}

const ModelAnimationQuaternionKeyFrame & ModelAnimationKeyFrames::GetQuaternionKeyFrame(int keyFrame) const
{
	return keyFrames[keyFrame].second;
}

void ModelAnimationKeyFrames::Load(BinaryReader * r)
{
	animationName = r->Wstring();

	UINT count = r->UInt();
	keyFrames.resize(count);
	for (UINT i = 0; i < count; i++)
	{
		keyFrames[i].first = r->Matrix();
		keyFrames[i].second.Load(r);
	}
}

void ModelAnimationQuaternionKeyFrame::Load(BinaryReader * r)
{
	q = r->QUATERNION();
	translation = r->Vector3();
}
