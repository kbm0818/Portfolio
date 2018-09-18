#pragma once

template <typename T>
class CBuffer;

class CBC_SkyPlane : public IComponent
{
public:
	CBC_SkyPlane();
	virtual ~CBC_SkyPlane();

	virtual void Update(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);

private:
	CBuffer<float>* cbuffer;

	float scale, brightness, translation;
};