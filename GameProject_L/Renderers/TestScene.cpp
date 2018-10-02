#include "stdafx.h"
#include "TestScene.h"

#include "Object/GameObject.h"
#include "Object/GameObjectFactory.h"

#include "Object\Fbx\FbxLoader.h" // 곧 지울꺼

TestScene::TestScene( GlobalValues* values )
	: Renderers( values )
{
	//게임 오브젝트 생성
	GameObject* object;
	/*object = values->GameObjectFactory->Create("SkyDome", values);
	gameObjects.push_back(object);*/

	object = values->GameObjectFactory->Create("SkyPlane", values);
	gameObjects.push_back(object);

	//object = values->GameObjectFactory->Create("Terrain", values);
	//gameObjects.push_back(object);

	////FbxLoader::Convert(Contents + L"FbxModels/parasite_l_starkie.fbx");
	////FbxLoader::ConvertAnim(Contents + L"FbxAnimation/JombieWalking.fbx");

	//object = values->GameObjectFactory->CreateModel(L"parasite_l_starkie", values);
	//gameObjects.push_back(object);

	
}

TestScene::~TestScene()
{
	for (GameObject* r : gameObjects)
		SAFE_DELETE(r);

	gameObjects.clear();
}

void TestScene::Update()
{
	for (GameObject* r : gameObjects)
		r->Update(values);
}

void TestScene::PreRender()
{
	for (GameObject* r : gameObjects)
		r->PreRender(values);
}

void TestScene::Render()
{
	for (GameObject* r : gameObjects)
		r->Render(values);
}

void TestScene::PostRender()
{
	for (GameObject* r : gameObjects)
		r->PostRender(values);
}

void TestScene::ResizeScreen()
{
	for (GameObject* r : gameObjects)
		r->ResizeScreen(values);
}