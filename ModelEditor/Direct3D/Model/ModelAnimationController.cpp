#include "../stdafx.h"
#include "ModelAnimationController.h"
#include "ModelAnimation.h"

ModelAnimationController::ModelAnimationController()
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

ModelAnimationController::~ModelAnimationController()
{
	for each(Pair temp in animations)
		SAFE_DELETE(temp.second);
	
	currentAnimation = NULL;
}

void ModelAnimationController::AddAnimation(ModelAnimation * animation)
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

ModelAnimation * ModelAnimationController::GetCurrentAnimation()
{
	if(currentAnimation != NULL)
		return currentAnimation;

	return NULL;
}

void ModelAnimationController::SetCurrentAnimation(wstring animationName)
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

void ModelAnimationController::SetCurrentAnimation(int index)
{
	currentAnimation = animations[index].second;
	
	Stop();
	Play();
}

UINT ModelAnimationController::GetAnimationCount()
{
	return animations.size();
}

ModelAnimation * ModelAnimationController::GetAnimation(int index)
{
	return animations[index].second;
}

ModelAnimation * ModelAnimationController::GetAnimation(wstring animationName)
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

void ModelAnimationController::ChangeOldThing()
{
	if (defaultAnimation == NULL)
		return;

	currentAnimation = defaultAnimation;
	useOld = false;

	Stop();
	Play();
}

void ModelAnimationController::SetDefaultAnim(wstring animationName)
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

void ModelAnimationController::UsePopAnimation(wstring animationName)
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

void ModelAnimationController::Play()
{
	animationMode = AnimationMode::Play;
}

void ModelAnimationController::Pause()
{
	animationMode = AnimationMode::Pause;
}

void ModelAnimationController::Stop()
{
	animationMode = AnimationMode::Stop;
	frameTimer = 0.0f;

	SetCurrentKeyFrame(0);
}

void ModelAnimationController::SetCurrentKeyFrame(int keyFrame)
{
	currentKeyFrame = (keyFrame) % currentAnimation->GetKeyFrames();
	nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();
	
	keyFrameFactor = 0.0f;
}

string ModelAnimationController::GetAnimationName(int index)
{
	return String::WStringToString(animations[index].second->GetName());
}

void ModelAnimationController::DeleteAnimation(int index)
{
	if (index > animations.size())
		return;

	Stop();
	currentAnimation = nullptr;

	SAFE_DELETE(animations[index].second);
	animations.erase(animations.begin() + index);
}

void ModelAnimationController::Update()
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
