#pragma once
#include "Components\ComponentBase.h"

class TestComponent : public ComponentBase
{
public:
	TestComponent() :ComponentBase() {}
	virtual ~TestComponent() {}

	virtual void RegistEvent();

	
private:
	void Test(IEventArgs& eventArgs);
};