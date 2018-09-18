#include "stdafx.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(wstring name, int keyFrame, float frameRate, float defaultFrameRate)
	: name(name), keyFrame(keyFrame), frameRate(frameRate * 2.0f), defaultFrameRate(defaultFrameRate), useRootMotion(false), useRepeat(true)
{
	
}

ModelAnimation::~ModelAnimation()
{
}
