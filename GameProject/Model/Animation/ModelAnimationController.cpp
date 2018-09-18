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
			// �ߺ� Ȯ��
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
		// �ߺ� Ȯ��
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

	//�ִϸ��̼��� FrameTimer�� ������ Frame����ð���ŭ �����ش�.
	frameTimer += Time::Delta();

	//currentAnimation�� FrameRate�� �ش� Animation�� ���� �� �𵨸� ���α׷����� �����ߴ� FrameRate��.
	//�� ���� 1.0f���� ������ ���������� �������� ���Ѵ�.
	float invFrameRate = 1.0f / currentAnimation->GetFrameRate();

	//FrameTimer�� 1������ ����ġ�� ������ ��� ���� ���������� �Ѿ��.
	while (frameTimer > invFrameRate)
	{
		currentKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrameCount();
		nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrameCount();

		frameTimer -= invFrameRate;
	}

	//KeyFrame�� �������� ����Ѵ�.
	//���� Frame�� ���� Frame ���̿��� ��� ���� �ð��� ����Ǿ������� ��Ÿ����.
	//frameTimer�� 0.19f�̰� invFrameRate�� 0.16f(60������)�̾��� ���
	//�������� ������ �� frameTimer�� 0.03f�� �ȴ�.
	//�׷��� 0.03 / 0.16 = 0.1875�� �����µ�..
	//�� ���� ���� ���� frameTimer�� 1������ �������� ��%������ ���Ѵ�.

	//�׷��ϱ� ���� ���� ���� 1�������� ������ 
	//18.75%���� �� ���� ������ �ִϸ��̼��� ���;� ��ũ�� �´� ���̴�.
	keyFrameFactor = frameTimer / invFrameRate;
}
