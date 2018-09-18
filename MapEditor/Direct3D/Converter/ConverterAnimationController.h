#pragma once

enum class AnimationMode
{
	Play = 0, Pause, Stop,
};

/********************************************************************************
 @breif
 Time�� ���� Animatiion�� Frame�� ����ϴ� Class
 
 ���� ���Ǵ� Animation �� �����ϰ�, �� Animation�� ���� Time�� �ش��ϴ� Key Frame(current, next)�� ���
 useQuaternionKeyFrames�� Ture�϶� Time�� ���� Frame �������� ���
********************************************************************************/
class ConverterAnimation;
class ConverterAnimationController
{
public:
	ConverterAnimationController();
	~ConverterAnimationController();

	void AddAnimation(ConverterAnimation* animation);

	void SetIsLoop(bool isLoop) { this->isLoop = isLoop; }

	ConverterAnimation* GetCurrentAnimation();
	void SetCurrentAnimation(wstring animationName);
	void SetCurrentAnimation(int index);

	UINT GetAnimationCount();
	ConverterAnimation* GetAnimation(int index);
	ConverterAnimation* GetAnimation(wstring animName);

	int GetCurrentKeyFrame() { return currentKeyFrame; }
	void SetCurrentKeyFrame(int keyFrame);
	
	int GetNextKeyFrame() { return nextKeyFrame; }
	float GetKeyFrameFactor() { return keyFrameFactor; }

	AnimationMode GetAnimationMode() { return animationMode; }

	void UseQuaternionKeyFrames(bool use) { useQuaternionKeyFrames = use; }
	bool UseQuaternionKeyFrames() { return useQuaternionKeyFrames; }

	void Play();
	void Pause();
	void Stop();

	void Update();
private:
	AnimationMode animationMode; /// ��� ���
	ConverterAnimation* currentAnimation; /// ���� Animation
	ConverterAnimation* prevAnimation; /// ���� Animation

	typedef pair<wstring, ConverterAnimation *> Pair;
	vector<Pair> animations; /// Animation ����
	
	float frameTimer; /// ���� KeyFrame���� ���� �� ��� �ð�

	int currentKeyFrame; /// ���� KeyFrame
	int nextKeyFrame; /// ���� KeyFrame
	float keyFrameFactor; /// ���� KeyFrame�� ���� KeyFrame���̿����� ���� ��
	bool useQuaternionKeyFrames; /// ���� ��� ���� flag

	bool isLoop;
};