#include "stdafx.h"
#include "MovingCube.h"
#include "Executes/Execute.h"
#include "Components/UserControllerC.h"
#include "Components/TransformC.h"

MovingCube::MovingCube()
{
	CreateData();
	CreateTexture();

	transform = new UserControllerC(nullptr);
}

MovingCube::~MovingCube()
{
	SAFE_DELETE(transform);
	SAFE_DELETE(buffer);
	SAFE_DELETE(mesh);
}

void MovingCube::Init(ExecuteValues * exValue)
{
}

void MovingCube::Update(ExecuteValues * exValue)
{
	transform->Update(exValue);
}

void MovingCube::Render(ExecuteValues * exValue)
{
	transform->Render(exValue);

	buffer->SetPSBuffer(1);

	shader->Render(ShaderType::VP);

	D3D::GetDC()->PSSetShaderResources(0, (UINT)views.size(), &views[0]);
	RenderOptions::Get()->SetSampler(0);

	mesh->Draw();

	shader->EndRender();
}

void MovingCube::PreRender(ExecuteValues * exValue)
{
}

void MovingCube::PostRender(ExecuteValues * exValue)
{
}

void MovingCube::ResizeScreen(ExecuteValues * exValue)
{
}

void MovingCube::CreateData()
{
	mesh = MeshManager::Get()->GetMesh<VertexType>(Contents + L"UserBinaryMeshes/Cube.bin", MeshType::BinaryCustumFile);
	buffer = new MovingCubeBuffer();
	shader = ShaderManager::Get()->GetShader(Shaders + L"POMTestRender.hlsl", ShaderType::VP);

}

void MovingCube::CreateTexture()
{
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_disp.png"));
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_normal.png"));
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"Box.png"));
}