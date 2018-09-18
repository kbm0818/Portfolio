#include "stdafx.h"
#include "SceneManager.h"
#include "IScene.h"
#include "World.h"

SceneManager* SceneManager::instance = nullptr;

void SceneManager::Update()
{
	//�� push, pop �޼����� �ִ��� Ȯ�� �� ó��
	for (unsigned int i = 0; i < messageQueue.size(); ++i)
	{
		if (messageQueue[i].kind == PUSH)
		{
			sceneList.push_back(messageQueue[i].scene);
			currentScene = sceneList.back();
			currentWorld = currentScene->GetWorld();
			currentScene->Awake();
		}
		else if (messageQueue[i].kind == POP)
		{
			delete sceneList.back();
			sceneList.pop_back();
			if (sceneList.size() > 0)
			{
				currentScene = sceneList.back();
				currentWorld = currentScene->GetWorld();
				currentScene->Start();
			}
		}
	}
	messageQueue.clear();

	if (sceneList.size() <= 0)
		GameExit();

	//���� �ֱٿ� ������ ���� ������Ʈ�Ѵ�.
	if(currentScene)
		currentScene->Update();
}

void SceneManager::LateUpdate()
{
	if (currentScene)
		currentScene->LateUpdate();
}

void SceneManager::PreRender()
{
	if (currentScene)
		currentScene->PreRender();
}

void SceneManager::Render()
{
	if (currentScene)
		currentScene->Render();
}

void SceneManager::PostRender()
{
	if (currentScene)
		currentScene->PostRender();
}

void SceneManager::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	instance->screenWidth = desc.Width;
	instance->screenHeight = desc.Height;

	//��� Scene���� Resize�� ȣ���Ѵ�. (ī�޶� Matrix ������Ʈ)
	for (size_t i = 0; i < sceneList.size(); ++i)
		sceneList[i]->ResizeScreen();
}

void SceneManager::PushScene(IScene * scene)
{
	if (scene == NULL) return;
	messageQueue.push_back(SCENE_MSG(PUSH, scene));
}

void SceneManager::PopScene()
{
	messageQueue.push_back(SCENE_MSG(POP, NULL));
}

void SceneManager::ChangeScene(IScene * scene)
{
	PopScene();
	PushScene(scene);
}

SceneManager * SceneManager::Get()
{
	return instance;
}

void SceneManager::Create()
{
	instance = new SceneManager;
	
	D3DDesc desc;
	D3D::GetDesc(&desc);

	instance->screenWidth = desc.Width;
	instance->screenHeight = desc.Height;
}

void SceneManager::Delete()
{
	SAFE_DELETE(instance);
}

SceneManager::~SceneManager()
{
	for (int i = sceneList.size() - 1; i >= 0; --i)
	{
		delete sceneList[i];
	}
	sceneList.clear();
	messageQueue.clear();
}