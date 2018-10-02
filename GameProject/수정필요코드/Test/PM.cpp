#include "stdafx.h"
#include "PM.h"
#include "Executes/Execute.h"

PM::PM(D3DXVECTOR3 position)
{
	CreateData();
	CreateTexture();
	
	this->position = position;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 50.0f, 50.0f, 50.0f);
	world = scale * world;
}

PM::~PM()
{
	SAFE_DELETE(pBuffer);
}

void PM::Update(ExecuteValues* exValue)
{
}

void PM::Render(ExecuteValues * exValue)
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

void PM::PostRender(ExecuteValues * exValue)
{
	ImGui::Begin("Environment");

	if (ImGui::CollapsingHeader("PM"))
	{
		ImGui::DragFloat("PMScale", (float *)&pBuffer->Data.heightScale, 0.001f, 0.0f, 0.1f);
	}

	ImGui::End();
}

void PM::PreRender(ExecuteValues * exValue)
{

}

void PM::CreateData()
{
	//mesh = (MeshData<VertexType> *)MeshManager::Get()->GetMesh(L"Quad");

	pBuffer = new PMBuffer();
	shader = ShaderManager::Get()->GetShader(Shaders + L"PMTestRender.hlsl", ShaderType::VP);
}

void PM::CreateTexture()
{
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_disp.png"));
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_normal.png"));
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"wood.png"));
}
