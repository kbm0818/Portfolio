#include "../stdafx.h"
#include "MoAnimationController.h"
#include "MoAnimation.h"

MoAnimationController::MoAnimationController()
{
	currentAnimation = NULL;
	currentKeyFrame = 0;
	nextKeyFrame = 0;
	keyFrameFactor = 0.0f;
	frameTimer = 0.0f;

	useQuaternionKeyFrames = true;
	useOld = false;
	animationMode = AnimationMode::Stop;

	curAnimName = L"";
}

MoAnimationController::~MoAnimationController()
{
	for each(Pair temp in animations)
		SAFE_DELETE(temp.second);
	
	currentAnimation = NULL;
}

void MoAnimationController::AddAnimation(MoAnimation * animation)
{
	bool isExist = false;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animation->GetName())
		{
			isExist = true;
			
			break;
		}
	}
	assert(isExist == false);

	animations.push_back(Pair(animation->GetName(), animation));
}

MoAnimation * MoAnimationController::GetCurrentAnimation()
{
	if(currentAnimation != NULL)
		return currentAnimation;

	return NULL;
}

void MoAnimationController::SetCurrentAnimation(wstring animationName)
{
	//이미 실행하고 있는 애니와 같다면 실행하지 않겠다
	if (wcscmp(curAnimName.c_str(),animationName.c_str()) == 0)
		return;

	curAnimName = animationName;
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}
	
	if (count != -1)
		currentAnimation = animations[count].second;
	else
		currentAnimation = NULL;

	Stop();
	Play();
}

void MoAnimationController::SetCurrentAnimation(int index)
{
	currentAnimation = animations[index].second;
	
	Stop();
}

UINT MoAnimationController::GetAnimationCount()
{
	return animations.size();
}

MoAnimation * MoAnimationController::GetAnimation(int index)
{
	return animations[index].second;
}

MoAnimation * MoAnimationController::GetAnimation(wstring animationName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}

	if (count != -1)
		return animations[count].second;
	else
		return NULL;
}

void MoAnimationController::ChangeOldThing()
{
	if (defaultAnimation == NULL)
		return;

	currentAnimation = defaultAnimation;
	useOld = false;

	Stop();
	Play();
}

void MoAnimationController::SetDefaultAnim(wstring animationName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}

	if (count == -1)
		return;

	defaultAnimation = animations[count].second;
}

void MoAnimationController::UsePopAnimation(wstring animationName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}

	if (count == -1)
		return;

	currentAnimation = animations[count].second; // 쓰려는걸로 바꾸고
	useOld = true;

	Stop();
	Play();
}

void MoAnimationController::Play()
{
	animationMode = AnimationMode::Play;
}

void MoAnimationController::Pause()
{
	animationMode = AnimationMode::Pause;
}

void MoAnimationController::Stop()
{
	animationMode = AnimationMode::Stop;
	frameTimer = 0.0f;

	SetCurrentKeyFrame(0);
}

void MoAnimationController::SetCurrentKeyFrame(int keyFrame)
{
	currentKeyFrame = (keyFrame) % currentAnimation->GetKeyFrames();
	nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();
	
	keyFrameFactor = 0.0f;
}

void MoAnimationController::Update()
{
	if (currentAnimation == NULL || animationMode != AnimationMode::Play)
		return;

	frameTimer += Time::Delta();

	float invFrameRate = 1.0f / currentAnimation->GetFrameRate();
	while (frameTimer > invFrameRate)
	{
		int keyFrame = currentAnimation->GetKeyFrames();

		if (useOld && ((currentKeyFrame + 1) / keyFrame >= 1)) // ㅇㅇ 원래대로 돌리기
			ChangeOldThing();
		
		if (currentAnimation->GetUseRepeat() == true)
		{
			currentKeyFrame = (currentKeyFrame + 1) % keyFrame;
			nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();	
		}
		else // Repeat가 아니라면
		{
			int tempKeyFrame = nextKeyFrame;

			currentKeyFrame = (currentKeyFrame + 1) % keyFrame;
			nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();

			if (nextKeyFrame < tempKeyFrame)
				currentKeyFrame = nextKeyFrame = tempKeyFrame;
		}
		frameTimer -= invFrameRate;
	}

	keyFrameFactor = frameTimer / invFrameRate;
}
