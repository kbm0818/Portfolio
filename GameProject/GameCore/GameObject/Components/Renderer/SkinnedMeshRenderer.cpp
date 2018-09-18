#include "stdafx.h"
#include "SkinnedMeshRenderer.h"
#include "../Animator/Animator.h"
#include "./GameCore/Resource/Mesh.h"
#include "./GameCore/Resource/Material.h"
#include "./MeshFilter.h"

SkinnedMeshRenderer::SkinnedMeshRenderer()
	: Renderer()
{
	this->material = nullptr;
	this->boneBuffer = nullptr;
	data.isSkinning = true;
	data.rootBoneIndex = 0;

	buffer = new ShaderBuffer(&data, sizeof(Data));
}


void SkinnedMeshRenderer::Init(Material * material, ShaderBuffer * boneBuffer, UINT rootBone, bool skinning)
{
	this->material = material;
	this->boneBuffer = boneBuffer;
	data.isSkinning = (float)skinning;
	data.rootBoneIndex = (float)rootBone;

	name = typeid(Renderer).name();
}

void SkinnedMeshRenderer::Awake()
{
	Renderer::Awake();
	shader = AssetManager::Get()->GetShader("BasicModel");
}

void SkinnedMeshRenderer::Update()
{
	Renderer::Update();
}

void SkinnedMeshRenderer::Render()
{
	if (!meshFilter) return;

	Mesh* mesh = meshFilter->GetMesh();
	mesh->BeginRender();
	{
		worldBuffer->SetVSBuffer(WORLD_REGISTER);
		boneBuffer->SetVSBuffer(2);
		buffer->SetVSBuffer(3);

		material->SetPsBuffer(1);

		ID3D11ShaderResourceView* diffuseView = material->GetDiffuseView();
		dc->PSSetShaderResources(0, 1, &diffuseView);

		shader->Render();
	}
	mesh->EndRender();
}