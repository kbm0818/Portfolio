#include "../stdafx.h"
#include "ModelAnimation.h"

ModelAnimation::ModelAnimation(wstring name, int keyFrame, float defaultFrameRate)
	: name(name), keyFrame(keyFrame), frameRate((float)keyFrame), defaultFrameRate(defaultFrameRate)
	, useRootMotion(false), useRepeat(true)
{
	
}

ModelAnimation::~ModelAnimation()
{
}
