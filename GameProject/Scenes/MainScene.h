#pragma once
#include "./GameCore/Scene/IScene.h"

class MainScene : public IScene
{
public:
	MainScene();
	virtual ~MainScene();

	virtual void Awake();
	virtual void Start();

	virtual void Update();
	virtual void LateUpdate();

	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

private:
	GameObject * paladin;
	GameObject* vanguard;

	GameObject* plane;
	GameObject* sky;
};