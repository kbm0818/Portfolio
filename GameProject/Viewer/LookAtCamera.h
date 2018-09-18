#pragma once
#include "Camera.h"

//class ModelScene;
class LookAtCamera : public Camera
{
public:
	LookAtCamera(float moveSpeed = 5.0f, float rotationSpeed = 60.0f);
	~LookAtCamera();

	void Update();
	//void SetTarget(ModelScene* target);

private:
	virtual void UpdateView();
	void UpdatePosition();

	float distance;
	float angle;
	float height;

	float moveSpeed;
	float rotationSpeed;
	//ModelScene* target;
};