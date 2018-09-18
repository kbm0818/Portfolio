#pragma once

class ModelAnimationKeyFrames;
class ModelAnimation
{
public:
	ModelAnimation(wstring name, int keyFrameCount, float defaultFrameRate);
	ModelAnimation(wstring name, int keyFrameCount, float defaultFrameRate, float frameRate);
	~ModelAnimation();

	ModelAnimation(ModelAnimation& copy);

	wstring GetName() { return name; }
	void SetName(wstring name) { this->name = name; }

	int GetKeyFrameCount() { return keyFrameCount; }

	float GetFrameRate() { return frameRate; }
	void SetFrameRate(float frameRate) { this->frameRate = frameRate; }

	float GetDefaultFrameRate() { return defaultFrameRate; }

	void ResetFrameRate() { frameRate = defaultFrameRate; }

	ModelAnimationKeyFrames* GetAnimationKeyFrames(int boneIndex);

private:
	friend class ModelLoader;

	wstring name;

	int keyFrameCount;
	float frameRate;
	float defaultFrameRate;

	//각 본들의 keyFrame별 위치 정보
	vector<ModelAnimationKeyFrames*> animationKeyFrames;
};
