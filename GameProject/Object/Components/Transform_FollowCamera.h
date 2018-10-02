#pragma once
#include "TransformC.h"

class Transform_FollowCamera : public TransformC
{
public:
	Transform_FollowCamera();
	~Transform_FollowCamera();

	virtual void Update(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);

private:

};