#pragma once

enum class GameObjectType : UINT
{
	SkyDome = 0,
	SkyPlane,
	Terrain,
	Monster,
	Player,

};

struct GlobalValues;
class IGameObject
{
public:

	virtual ~IGameObject() {};

	virtual void Update(GlobalValues* exValue) = 0;
	virtual void PreRender(GlobalValues* exValue) = 0;
	virtual void Render(GlobalValues* exValue) = 0;
	virtual void PostRender(GlobalValues* exValue) = 0;
	virtual void ResizeScreen(GlobalValues* exValue) = 0;
};