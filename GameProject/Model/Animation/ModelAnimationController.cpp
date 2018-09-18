#include "stdafx.h"
#include "./ModelAnimationController.h"
#include "./ModelAnimation.h"

ModelAnimationController::ModelAnimationController()
{
	currentAnimation = NULL;
	currentKeyFrame = 0;
	nextKeyFrame = 0;
	keyFrameFactor = 0.0f;
	frameTimer = 0.0f;

	useQuaternionKeyFrames = true;

	animationMode = AnimationMode::Stop;
}

ModelAnimationController::~ModelAnimationController()
{
	for each(Pair temp in animations)
		SAFE_DELETE(temp.second);

	currentAnimation = NULL;
}

void ModelAnimationController::AddAnimation(ModelAnimation * animation)
{
	wstring animationName = animation->GetName();

	bool bExist = true;
	while (bExist)
	{
		bExist = false;
		for (size_t i = 0; i < animations.size(); i++)
		{
			// 중복 확인
			if (animations[i].first == animationName)
			{
				animationName.append(L" - copy");
				bExist = true;
				break;
			}
		}
	}

	animation->SetName(animationName);
	animations.push_back(Pair(animationName, animation));
}

bool ModelAnimationController::isExistAnimation(wstring name)
{
	bool bExist = false;
	for (size_t i = 0; i < animations.size(); i++)
	{
		// 중복 확인
		if (animations[i].first == name)
		{
			bExist = true;
			break;
		}
	}
	return bExist;
}

void ModelAnimationController::SetAnimationName(int index, wstring name)
{
	animations[index].first = name;
	animations[index].second->SetName(name);
}

ModelAnimation * ModelAnimationController::GetCurrentAnimation()
{
	if (currentAnimation != NULL)
		return currentAnimation;

	return NULL;
}

int ModelAnimationController::GetCurrentAnimationIndex()
{
	for (size_t i = 0; i < animations.size(); ++i)
	{
		if (currentAnimation == animations[i].second)
			return i;
	}
	return -1;
}

void ModelAnimationController::SetCurrentAnimation(wstring animationName)
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
		currentAnimation = animations[count].second;
	else
		currentAnimation = NULL;

	Stop();
}

void ModelAnimationController::SetCurrentAnimation(int index)
{
	if (index < 0 || index >= (INT)animations.size())
		currentAnimation = NULL;
	else
		currentAnimation = animations[index].second;

	Stop();
}

UINT ModelAnimationController::GetAnimationCount()
{
	return animations.size();
}

ModelAnimation * ModelAnimationController::GetAnimation(int index)
{
	if (index < 0 || index >= (INT)animations.size())
		return NULL;
	else
		return animations[index].second;
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
	currentKeyFrame = (keyFrame) % currentAnimation->GetKeyFrameCount();
	nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrameCount();

	keyFrameFactor = 0.0f;
}

void ModelAnimationController::Update()
{
	if (currentAnimation == NULL || animationMode != AnimationMode::Play)
		return;

	//애니메이션의 FrameTimer를 게임의 Frame경과시간만큼 더해준다.
	frameTimer += Time::Delta();

	//currentAnimation의 FrameRate는 해당 Animation을 만들 때 모델링 프로그램에서 지정했던 FrameRate다.
	//이 것을 1.0f으로 나눠서 실질적으로 몇초인지 구한다.
	float invFrameRate = 1.0f / currentAnimation->GetFrameRate();

	//FrameTimer가 1프레임 기준치를 지났을 경우 다음 프레임으로 넘어간다.
	while (frameTimer > invFrameRate)
	{
		currentKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrameCount();
		nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrameCount();

		frameTimer -= invFrameRate;
	}

	//KeyFrame의 보간값을 계산한다.
	//현재 Frame과 다음 Frame 사이에서 어느 정도 시간이 경과되었는지를 나타낸다.
	//frameTimer가 0.19f이고 invFrameRate가 0.16f(60프레임)이었을 경우
	//프레임이 지나간 뒤 frameTimer는 0.03f이 된다.
	//그러면 0.03 / 0.16 = 0.1875가 나오는데..
	//이 값은 현재 남은 frameTimer가 1프레임 단위에서 몇%인지를 뜻한다.

	//그러니까 위와 같은 경우는 1프레임이 지나고 
	//18.75%정도 더 지난 상태의 애니메이션이 나와야 싱크가 맞는 것이다.
	keyFrameFactor = frameTimer / invFrameRate;
}
