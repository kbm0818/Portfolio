#include "stdafx.h"
#include "Program.h"

#include "./Renderers/Renderers.h"
#include "./Renderers/TestScene.h"
#include "./Renderers/GUI.h"

#include "./Systems/Camera/FirstPerson.h"
#include "./Systems/Camera/Perspective.h"
#include "./Systems/Camera/Orthographic.h"
#include "./Systems/Camera/Viewport.h"

#include "./Systems/GameSystems/TerrainHeightData.h"
#include "./Systems/GameSystems/Frustum.h"

#include "./Systems/Renders/RenderOptions.h"

#include "Object/GameObjectFactory.h"
#include "Object/Shader/ShaderFactory.h"
#include "Object/SRV/SRVFactory.h"

#define SCREEN_DEPTH 1000.0f

Program::Program()
{
	Init();

	renderer.push_back(new TestScene(values));
	renderer.push_back(new Gui(values));
}

Program::~Program()
{
	for (Renderers* r : renderer)
		SAFE_DELETE(r);

	SAFE_DELETE(values->Frustum);
	SAFE_DELETE(values->GuiSettings);
	SAFE_DELETE(values->GlobalLight);
	SAFE_DELETE(values->GlobalViewProjection);
	SAFE_DELETE(values->GlobalCamera);
	SAFE_DELETE(values->Perspective);
	SAFE_DELETE(values->MainCamera);
	SAFE_DELETE(values->Viewport);
	SAFE_DELETE(values->GameObjectFactory);
	SAFE_DELETE(values->ShaderFactory);
	SAFE_DELETE(values->SRVFactory);
	SAFE_DELETE(values->TerrainHeightData);
	SAFE_DELETE(values->RenderOption);
	SAFE_DELETE(values);
}

void Program::Init()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values = new GlobalValues();
	values->GuiSettings = new GuiSettings();

	values->GlobalLight = new GlobalLightBuffer();
	values->GlobalViewProjection = new ViewProjectionBuffer();
	values->GlobalCamera = new GlobalCameraBuffer();

	values->Perspective = new Perspective(desc.Width, desc.Height);
	values->Viewport = new Viewport(desc.Width, desc.Height);
	values->MainCamera = new FirstPerson();
	values->MainCamera->SetPosition(-1, -1, -1);
	values->MainCamera->SetRotationDegree(D3DXVECTOR2(-50, 50));

	values->Frustum = new Frustum();
	values->TerrainHeightData = new TerrainHeightData();
	//values->TerrainHeightData->Init(Textures + L"heightmap.bmp", HeightMapFileType::Texture);
	//values->TerrainHeightData->SaveBinary(Contents + L"UserBinaryTerrains/baseTerrain.map");
	values->TerrainHeightData->Init(Contents + L"UserBinaryTerrains/Terrain0.map", HeightMapFileType::binary);

	values->GameObjectFactory = new GameObjectFactory();
	values->ShaderFactory = new ShaderFactory();
	values->SRVFactory = new SRVFactory();

	values->RenderOption = new RenderOptions;

}

void Program::Update()
{
	values->MainCamera->Update();

	for (Renderers* r : renderer)
		r->Update();
}

void Program::PreRender()
{
	SetGlobalValues();

	for (Renderers* r : renderer)
		r->PreRender();
}

void Program::Render()
{
	SetGlobalValues();

	for (Renderers* r : renderer)
		r->Render();
}

void Program::PostRender()
{
	for (Renderers* r : renderer)
		r->PostRender();
}

void Program::ResizeScreen()
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	values->Perspective->Set(desc.Width, desc.Height);
	values->Viewport->Set(desc.Width, desc.Height);

	for (Renderers* r : renderer)
		r->ResizeScreen();
}

void Program::SetGlobalValues()
{
	D3DXMATRIX view, projection, reflection;
	D3DXVECTOR3 cameraPosition;

	values->MainCamera->GetMatrix(&view);
	values->Perspective->GetMatrix(&projection);
	values->MainCamera->GetReflectionMatrix(&reflection);
	values->MainCamera->GetPosition(&cameraPosition);

	////////////////////////////////////////////////////////////////////

	values->GlobalViewProjection->SetView(view);
	values->GlobalViewProjection->SetProjection(projection);
	values->GlobalViewProjection->SetReflection(reflection);
	values->GlobalViewProjection->SetVSBuffer(0);

	values->GlobalLight->SetPSBuffer(0);

	values->GlobalCamera->Data.position = cameraPosition;
	values->GlobalCamera->SetVSBuffer(2);

	values->Frustum->ConstructFrustum(SCREEN_DEPTH, projection, view);
}