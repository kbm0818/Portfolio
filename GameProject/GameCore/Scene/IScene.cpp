#include "stdafx.h"
#include "IScene.h"
#include "World.h"

IScene::~IScene()
{
	SAFE_DELETE(world);
}

void IScene::Awake()
{
}

void IScene::Start()
{
}

void IScene::Update()
{
	if (world)
		world->Update();
}

void IScene::LateUpdate()
{
	if (world)
		world->LateUpdate();
}

void IScene::PreRender()
{
	if (world)
		world->PreRender();
}

void IScene::Render()
{
	if (world)
		world->Render();
}

void IScene::PostRender()
{
	if (world)
		world->PostRender();
}

void IScene::ResizeScreen()
{
	if (world)
		world->ResizeScreen();
}

void IScene::CreateWorld(int areaSize, int areaCount, int heightSize, int heightCount)
{
	SAFE_DELETE(world);
	world = new World(this, areaSize, areaCount, heightSize, heightCount);
}
