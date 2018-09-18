#include "stdafx.h"
#include "MainScene.h"

#include "../Environment/SkyShader.h"
#include "../Scripts/AttachTarget.h"

#include "GameCore\GameObject\Components\Animator\Animator.h"
#include "Model\Animation\ModelAnimationController.h"
#include "Model\Animation\ModelAnimation.h"
#include "GameCore\2DObject\MeshMaker.h"

#include "GameCore\GameObject\TestComponent.h"
#include "Scripts\HitCheck.h"

MainScene::MainScene()
{
	CreateWorld(16, 4, 16, 1); // (int areaSize, int areaCount, int heightSize, int heightCount)
}

MainScene::~MainScene()
{

}

void MainScene::Awake()
{
	D3DXVECTOR3 center = WORLD->GetCenterPosition();

	//Shader
	AssetManager::Get()->AddShader<SkyShader, Vertex>("SkyShader", Shaders + L"Sky.hlsl");

	//Mesh
	AssetManager::Get()->AddMesh("SkySphere", MeshMaker::GetCube<Vertex>(1.0f));

	//Material
	AssetManager::Get()->SetupMesh("SkySphere", "", &sky);

	Renderer* skyRenderer = sky->GetComponent<Renderer>();
	skyRenderer->Init("SkyShader", "");

	//World에 할당
	WORLD->AddGlobalObject(sky);
}

//void MainScene::Awake()
//{
//	D3DXVECTOR3 center = WORLD->GetCenterPosition();
//
//	//Shader
//	AssetManager::Get()->AddShader<SkyShader, Vertex>("SkyShader", Shaders + L"Sky.hlsl");
//
//	////Model
//	//AssetManager::Get()->AddModel("Paladin", String::WStringToString(Contents) + "\\BinModels\\Paladin0402.model");
//	//AssetManager::Get()->AddModel("Vanguard", String::WStringToString(Contents) + "\\BinModels\\Vanguard0402.model");
//
//	//Mesh
//	AssetManager::Get()->AddMesh("Plane", MeshMaker::GetPlane<VertexTexture>(16, 16, 16, 16));
//	AssetManager::Get()->AddMesh("SkySphere", MeshMaker::GetSphere<Vertex>(1.0f));
//	//Material
//	AssetManager::Get()->AddMaterial("PlaneMaterial", Contents + L"Textures\\Box.png");
//
//	//AssetManager::Get()->SetupToLoadedModel("Paladin", &paladin);
//	//AssetManager::Get()->SetupToLoadedModel("Vanguard", &vanguard);
//	AssetManager::Get()->SetupMesh("Plane", "PlaneMaterial", &plane);
//	GameObject* plane2;
//	GameObject* plane3;
//	AssetManager::Get()->SetupMesh("Plane", "PlaneMaterial", &plane2);
//	AssetManager::Get()->SetupMesh("Plane", "PlaneMaterial", &plane3);
//
//	AssetManager::Get()->SetupMesh("SkySphere", "", &sky);
//
//	//vanguard->SetPosition({ center.x - 5, center.y, center.z });
//	//Animator* vanguardAnim = vanguard->GetComponent<Animator>();
//	//vanguardAnim->GetAnimationController()->SetCurrentAnimation(L"Idle");
//	//vanguardAnim->GetAnimationController()->Play();
//
//	//Animator* anim = paladin->GetComponent<Animator>();
//	//anim->GetAnimationController()->SetCurrentAnimation(L"Idle");
//	//anim->GetAnimationController()->Play();
//
//	Renderer* skyRenderer = sky->GetComponent<Renderer>();
//	skyRenderer->Init("SkyShader", "");
//
//	Renderer* planeRenderer = plane->GetComponent<Renderer>();
//	planeRenderer->Init("BasicTexture", "");
//
//	//World에 할당
//	WORLD->AddGlobalObject(sky);
//	WORLD->AddGlobalObject(plane);
//	WORLD->AddGlobalObject(plane2);
//	WORLD->AddGlobalObject(plane3);
//
//	plane2->SetPosition(D3DXVECTOR3(8, 0, 8));
//	plane3->SetPosition(D3DXVECTOR3(64 - 8, 0, 64 - 8));
//
//	//WORLD->AddObject(World::eAlly, vanguard, { 32 - 5, 0, 32 + 5 });
//	//WORLD->AddObject(World::eAlly, paladin);
//
//	//vanguard->AddComponent<HitCheck>();
//
//}

void MainScene::Start()
{
}

void MainScene::Update()
{
	//auto* ctrl = paladin->GetComponent<Animator>()->GetAnimationController();
	//float angle = 0.0f;
	//bool pressed[4] = { false };
	//if (Keyboard::Get()->Press(VK_UP))		pressed[0] = true;
	//if (Keyboard::Get()->Press(VK_DOWN))	pressed[1] = true;
	//if (Keyboard::Get()->Press(VK_LEFT))	pressed[2] = true;
	//if (Keyboard::Get()->Press(VK_RIGHT))	pressed[3] = true;

	//if (pressed[0]) angle = 0.0f;
	//if (pressed[1]) angle = 180.0f;
	//if (pressed[2]) angle = -90.0f;
	//if (pressed[3]) angle = 90.0f;

	//if (pressed[0] && pressed[3]) angle = 45.0f;
	//if (pressed[0] && pressed[2]) angle = -45.0f;
	//if (pressed[1] && pressed[3]) angle = 135.0f;
	//if (pressed[1] && pressed[2]) angle = -135.0f;

	//if (pressed[0] || pressed[1] || pressed[2] || pressed[3])
	//{
	//	if (ctrl->GetCurrentAnimation()->GetName() != L"Walk")
	//	{
	//		ctrl->SetCurrentAnimation(L"Walk");
	//		ctrl->Play();
	//	}

	//	D3DXVECTOR3 pos = paladin->GetPosition();
	//	pos += paladin->Forward() * 3.0f * Time::Delta();
	//	paladin->SetPosition(pos);

	//	paladin->SetRotation(QuaternionUtility::RotateTowards(
	//		paladin->GetRotation(), D3DXVECTOR3(0, angle, 0), 720 * Time::Delta()
	//	));
	//}
	//else
	//{
	//	if (ctrl->GetCurrentAnimation()->GetName() != L"Slash" && ctrl->GetCurrentAnimation()->GetName() != L"Idle")
	//	{
	//		ctrl->SetCurrentAnimation(L"Idle");
	//		ctrl->Play();
	//	}
	//}

	//if (Keyboard::Get()->Down(VK_SPACE))
	//{
	//	ctrl->SetCurrentAnimation(L"Slash");
	//	ctrl->Play();
	//}

	IScene::Update();
}

void MainScene::LateUpdate()
{
	D3DXVECTOR3 cameraPos;
	WORLD->GetMainCamera()->GetPosition(&cameraPos);
	sky->SetPosition(cameraPos);
	
	IScene::LateUpdate();
}

void MainScene::PreRender()
{
	IScene::PreRender();
}

void MainScene::Render()
{
	IScene::Render();
}

void MainScene::PostRender()
{
	IScene::PostRender();

	//D3DXQUATERNION q = paladin->GetRotation();
	//ImGui::Text("x: %f, y: %f, z: %f, w: %f", q.x, q.y, q.z, q.w);

	//D3DXVECTOR3 forward = paladin->Forward();
	//ImGui::Text("Forward=> x: %f, y: %f, z: %f", forward.x, forward.y, forward.z);
}