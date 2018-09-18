#include "stdafx.h"
#include "Renderer.h"
#include "./GameCore/Resource/Mesh.h"
#include "./GameCore/Resource/Material.h"
#include "./MeshFilter.h"


void Renderer::Init(string shaderName, string materialName)
{
	this->shader = AssetManager::Get()->GetShader(shaderName);
	this->material = AssetManager::Get()->GetMaterial(materialName);
}

void Renderer::Awake()
{
	meshFilter = gameObject->GetComponent<MeshFilter>();
}

void Renderer::Update()
{
	worldBuffer->SetMatrix(gameObject->GetTransform()->GetWorldMatrix());
}

void Renderer::Render()
{
	if (!meshFilter) return;

	Mesh* mesh = meshFilter->GetMesh();
	mesh->BeginRender();
	{
		worldBuffer->SetVSBuffer(WORLD_REGISTER);
		material->SetPsBuffer(1);

		ID3D11ShaderResourceView* diffuseView = material->GetDiffuseView();
		States::SetSamplerDefault(0, 1);
		dc->PSSetShaderResources(0, 1, &diffuseView);

		shader->Render();
	}

	/*States::SetDepthOff();
	States::SetRasterOffCullMode();
	{*/
		mesh->EndRender();
	/*}
	States::SetDepthDefault();
	States::SetRasterClock();*/
}

void Renderer::PostRender()
{
	if (shader)
		shader->PostRender();
}

void Renderer::SetShader(string shaderName)
{
	this->shader = AssetManager::Get()->GetShader(shaderName);
}
