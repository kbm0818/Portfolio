#include "stdafx.h"
#include "Program.h"

#include "./Viewer/LookAtCamera.h"
#include "./Scenes/MainScene.h"

Program::Program()
{
	States::Create();
	SceneManager::Create();

	SceneManager::Get()->PushScene(new MainScene);
}

Program::~Program()
{	
	SceneManager::Delete();
	States::Delete();
}

void Program::Update()
{
	SceneManager::Get()->Update();
}

void Program::LateUpdate()
{
	SceneManager::Get()->LateUpdate();
}

void Program::PreRender()
{
	SceneManager::Get()->PreRender();
}

void Program::Render()
{
	SceneManager::Get()->Render();
}

void Program::PostRender()
{
	SceneManager::Get()->PostRender();
}

void Program::ResizeScreen()
{
	SceneManager::Get()->ResizeScreen();
}
