#pragma once

enum class AnimationMode
{
	Play = 0, Pause, Stop,
};

class MoAnimation;
class ModelAnimationController;
class MoAnimationController
{
public:
	MoAnimationController();
	~MoAnimationController();

	void AddAnimation(MoAnimation* animation);

	MoAnimation* GetCurrentAnimation();
	void SetCurrentAnimation(wstring animationName);
	void SetCurrentAnimation(int index);

	UINT GetAnimationCount();
	MoAnimation* GetAnimation(int index);
	MoAnimation* GetAnimation(wstring animationName);

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

	void Update();

	static void Read(BinaryReader* r, ModelAnimationController* modelSkeleton);
	static void Write(BinaryWriter* w, ModelAnimationController* modelSkeleton);


private:
	AnimationMode animationMode;
	MoAnimation* currentAnimation;

	typedef pair<wstring, MoAnimation *> Pair;
	vector<Pair> animations;
	
	float frameTimer;

	int currentKeyFrame;
	int nextKeyFrame;
	float keyFrameFactor;

	bool useQuaternionKeyFrames;

	//»ª¾Ö´Ï Ãß°¡
	MoAnimation* defaultAnimation;
	bool useOld;

	wstring curAnimName;
};