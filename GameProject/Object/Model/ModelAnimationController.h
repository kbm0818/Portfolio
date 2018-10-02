#pragma once

enum class AnimationMode
{
	Play = 0, Pause, Stop,
};

class ModelAnimation;
class ModelAnimationController
{
public:
	ModelAnimationController();
	~ModelAnimationController();

	void AddAnimation(ModelAnimation* animation);

	ModelAnimation* GetCurrentAnimation();
	void SetCurrentAnimation(wstring animationName);
	void SetCurrentAnimation(int index);

	UINT GetAnimationCount();
	ModelAnimation* GetAnimation(int index);
	ModelAnimation* GetAnimation(wstring animationName);

	void ChangeOldThing();
	void SetDefaultAnim(wstring animationName);
	void UsePopAnimation(wstring animationName);

	void Play();
	void Pause();
	void Stop();

	int GetCurrentKeyFrame() { return currentKeyFrame; }
	void SetCurrentKeyFrame(int keyFrame);
	
	int GetNextKeyFrame() { return nextKeyFrame; }
	float GetKeyFrameFactor() { return keyFrameFactor; }

	AnimationMode GetAnimationMode() { return animationMode; }

	void UseQuaternionKeyFrames(bool use) { useQuaternionKeyFrames = use; }
	bool UseQuaternionKeyFrames() { return useQuaternionKeyFrames; }

	bool GetUseOld() { return useOld; }

	string GetAnimationName(int index);
	void DeleteAnimation(int index);

	void Update();

private:
	AnimationMode animationMode;
	ModelAnimation* currentAnimation;

	typedef pair<wstring, ModelAnimation *> Pair;
	vector<Pair> animations;
	
	float frameTimer;

	int currentKeyFrame;
	int nextKeyFrame;
	float keyFrameFactor;

	bool useQuaternionKeyFrames;

	//»ª¾Ö´Ï Ãß°¡
	ModelAnimation* defaultAnimation;
	bool useOld;

	wstring curAnimName;
};