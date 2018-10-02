#include "stdafx.h"
#include "MoAnimation.h"

MoAnimation::MoAnimation(wstring name, int keyFrame, float defaultFrameRate)
	: name(name), keyFrame(keyFrame), frameRate((float)keyFrame), defaultFrameRate(defaultFrameRate), useRootMotion(false), useRepeat(true)
{
	
}

MoAnimation::~MoAnimation()
{
}
