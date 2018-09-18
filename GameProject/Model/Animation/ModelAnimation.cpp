#include "stdafx.h"
#include "ModelAnimation.h"
#include "ModelAnimationKeyFrames.h"


ModelAnimation::ModelAnimation(wstring name, int frameCount, float defaultFrameRate)
	: name(name), keyFrameCount(frameCount), frameRate(defaultFrameRate), defaultFrameRate(defaultFrameRate)
{

}

ModelAnimation::ModelAnimation(wstring name, int keyFrameCount, float defaultFrameRate, float frameRate)
	: name(name), keyFrameCount(keyFrameCount), frameRate(frameRate), defaultFrameRate(defaultFrameRate)
{
}

ModelAnimation::~ModelAnimation()
{
	for (ModelAnimationKeyFrames* temp : animationKeyFrames)
		SAFE_DELETE(temp);
}

ModelAnimation::ModelAnimation(ModelAnimation & copy)
{
	name = copy.name;
	keyFrameCount = copy.keyFrameCount;
	frameRate = copy.frameRate;
	defaultFrameRate = copy.defaultFrameRate;

	for (size_t i = 0; i < copy.animationKeyFrames.size(); ++i)
	{
		animationKeyFrames.push_back(new ModelAnimationKeyFrames(*copy.animationKeyFrames[i]));
	}
}

ModelAnimationKeyFrames * ModelAnimation::GetAnimationKeyFrames(int boneIndex)
{
	return animationKeyFrames[boneIndex];
}
