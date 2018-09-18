#include "stdafx.h"
#include "TestComponent.h"

void TestComponent::RegistEvent()
{
	gameObject->AddEvent("OnTest", name, [this](IEventArgs& e)
	{
		this->Test(e);
	});
}

void TestComponent::Test(IEventArgs & eventArgs)
{
	OutputDebugStringA("Test\n");
}
