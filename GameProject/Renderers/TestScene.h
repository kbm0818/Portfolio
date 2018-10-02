#pragma once
#include "Renderers.h"

class GameObject;
class TestScene : public Renderers
{
public:
	TestScene( GlobalValues* values );
	~TestScene();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	vector<IGameObject* > gameObjects;
};