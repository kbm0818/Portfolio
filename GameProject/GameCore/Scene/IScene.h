#pragma once

class World;
class IScene
{
public:
	IScene() : world(nullptr) {}
	~IScene();

	virtual void Awake();
	virtual void Start();

	virtual void Update();
	virtual void LateUpdate();

	virtual void PreRender();
	virtual void Render();	
	virtual void PostRender();

	virtual void ResizeScreen();

	World* GetWorld() { return world; }
protected:
	void CreateWorld(int areaSize, int areaCount, int heightSize, int heightCount);
	World* world;
};