#pragma once

class ModelAnimation
{
public:
	ModelAnimation(wstring name, int keyFrame, float defaultFrameRate);
	~ModelAnimation();

	wstring GetName() { return name; }
	int GetKeyFrames() { return keyFrame; }

	float GetFrameRate() { return frameRate; }
	void SetFrameRate(float frameRate) { frameRate = frameRate; }

	float GetDefaultFrameRate() { return defaultFrameRate; }

	void ResetFrameRate() { frameRate = defaultFrameRate; }

	void OnRootMotion()
	{
		useRootMotion = true;
	}

	void OffRootMotion()
	{
		useRootMotion = false;
	}

	bool GetUseRootMotion()
	{
		return useRootMotion;
	}

	bool GetUseRepeat()
	{
		return useRepeat;
	}

	void OnNotRepeatMotion()
	{
		useRepeat = false;
	}

private:
	wstring name;

	int keyFrame;
	float frameRate;
	float defaultFrameRate;

	bool useRootMotion;
	bool useRepeat;
};
