#pragma once
#include "Renderers.h"

class Gui : public Renderers
{
public:
	Gui(GlobalValues* values);
	~Gui();

	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen();

private:
	void ChangeShowDemoWindow();
	void ChangeShowEnvironmentWindow();
	void ChangeShowSystemInfoWindow();
};