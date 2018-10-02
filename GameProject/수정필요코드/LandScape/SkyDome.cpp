#include "stdafx.h"
#include "SkyDome.h"
#include "../Utilities/Mesh.h"
#include "Components\TransformC.h"
#include "Components\MaterialComponent.h"
#include "Executes\Execute.h"
#include "Viewer\Camera.h"

SkyDome::SkyDome()
	: mesh(nullptr), cBuffer(nullptr)
{
}

SkyDome::~SkyDome()
{
	SAFE_DELETE(cBuffer);
}

void SkyDome::Init(ExecuteValues * exValue)
{
	mesh = MeshManager::Get()->GetMesh<VertexType>(Contents + L"Meshes/Sphere.data", MeshType::BinaryMeshFile);

	cBuffer = new SkyDomeBuffer();

	components.push_back(new TransformC);

	//components.push_back(new MaterialComponent);
	//((MaterialComponent*)components[ComponentID::MaterialComponent])->SetShader(
	//	ShaderManager::Get()->GetShader(Shaders + L"Sky.hlsl", ShaderType::VP)
	//	,ShaderType::VP
	//);
}

void SkyDome::Update(ExecuteValues * exValue)
{
	D3DXVECTOR3 position;
	exValue->MainCamera->GetPosition(&position);
	
	//((Transform*)components[TransformComponentID])->SetPosition(position);

	for (IComponent* c : components)
		c->Update(exValue);
}

void SkyDome::PreRender(ExecuteValues * exValue)
{
	for (IComponent* c : components)
		c->PreRender(exValue);
}

void SkyDome::Render(ExecuteValues * exValue)
{
	RenderOptions::Get()->TurnOffZBuffer();
	RenderOptions::Get()->TurnOffCullMode();
	{
		cBuffer->SetPSBuffer(2);
		for (IComponent* c : components)
			c->Render(exValue);
		mesh->Draw();
	}
	RenderOptions::Get()->TurnOnZBuffer();
	RenderOptions::Get()->TurnOnCullMode();
}

void SkyDome::PostRender(ExecuteValues * exValue)
{
	if (exValue->GuiSettings->bShowEnvironmentWindow == true)
	{
		ImGui::Begin("Environment");

		if (ImGui::CollapsingHeader("Sky"))
		{
			ImGui::ColorEdit3("Center", (float *)&cBuffer->Data.Center);
			ImGui::ColorEdit3("Apex", (float *)&cBuffer->Data.Apex);
		}

		ImGui::End();
	}

	for (IComponent* c : components)
		c->PostRender(exValue);
}

void SkyDome::ResizeScreen(ExecuteValues * exValue)
{
}
