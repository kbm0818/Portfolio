#include "stdafx.h"
#include "ModelAnimationKeyFrames.h"

ModelAnimationKeyFrames::ModelAnimationKeyFrames(wstring animationName)
	: animationName(animationName)
{

}

ModelAnimationKeyFrames::~ModelAnimationKeyFrames()
{

}

ModelAnimationKeyFrames::ModelAnimationKeyFrames(ModelAnimationKeyFrames & copy)
{
	animationName = copy.animationName;
	for (size_t i = 0; i < copy.keyFrames.size(); ++i)
	{
		keyFrames.push_back(copy.keyFrames[i]);
	}
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

ModelAnimationQuaternionKeyFrame::ModelAnimationQuaternionKeyFrame(const ModelAnimationQuaternionKeyFrame & copy)
{
	q = copy.q;
	translation = copy.translation;
}
