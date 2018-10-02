#include "stdafx.h"
#include "POM.h"
#include "Executes/Execute.h"

POM::POM(D3DXVECTOR3 position)
{
	CreateData();
	CreateTexture();
	
	this->position = position;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 50.0f, 50.0f, 50.0f);
	world = scale * world;
}

POM::~POM()
{
	SAFE_DELETE(pBuffer);
}

void POM::Update(ExecuteValues* exValue)
{
}

void POM::Render(ExecuteValues * exValue)
{
	//exValue->worldBuffer->SetMatrix(world);
	//exValue->worldBuffer->SetVSBuffer(1);

	pBuffer->SetPSBuffer(1);

	shader->Render(ShaderType::VP);

	D3D::GetDC()->PSSetShaderResources(0, (UINT)views.size(), &views[0]);
	RenderOptions::Get()->SetSampler(0);

	mesh->Draw();

	shader->EndRender();
}

void POM::PostRender(ExecuteValues * exValue)
{
	ImGui::Begin("Environment");

	if (ImGui::CollapsingHeader("POM"))
	{
		ImGui::DragFloat("POMScale", (float *)&pBuffer->Data.heightScale, 0.001f, 0.0f, 0.1f);
	}

	ImGui::End();
}

void POM::PreRender(ExecuteValues * exValue)
{

}


void POM::CreateData()
{
	//mesh = (MeshData<VertexType> *)MeshManager::Get()->GetMesh(L"Quad");

	pBuffer = new POMBuffer();
	shader = ShaderManager::Get()->GetShader(Shaders + L"POMTestRender.hlsl", ShaderType::VP);
}

void POM::CreateTexture()
{
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_disp.png"));
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_normal.png"));
	//views.push_back(SRVManager::Get()->GetSRV(Textures + L"wood.png"));
	views.push_back(SRVManager::Get()->GetSRV(Contents + L"FbxModels/mutant.fbm/Mutant_diffuse.png"));
}
