#include "stdafx.h"
#include "Tesselation.h"
#include "Utilities/Mesh.h"
#include "Executes/Execute.h"

Tesselation::Tesselation(D3DXVECTOR3 position)
{
	CreateData();
	CreateTexture();
	
	this->position = position;
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 50.0f, 50.0f, 50.0f);
	world = scale * world;
}

Tesselation::~Tesselation()
{
	SAFE_DELETE(pBuffer);
}

void Tesselation::Update(ExecuteValues* exValue)
{

}

void Tesselation::Render(ExecuteValues* exValue)
{
	exValue->ViewProjection->SetDSBuffer(0);
	//exValue->worldBuffer->SetMatrix(world);
	//exValue->worldBuffer->SetDSBuffer(1);

	pBuffer->SetDSBuffer(2);

	shader->Render(ShaderType::VHDP);

	D3D::GetDC()->DSSetShaderResources(0, (UINT)views.size(), &views[0]);
	D3D::GetDC()->PSSetShaderResources(0, (UINT)views.size(), &views[0]);
	RenderOptions::Get()->SetSampler(0);

	mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

	shader->EndRender();
}

void Tesselation::PostRender(ExecuteValues* exValue)
{
	ImGui::Begin("Environment");

	if (ImGui::CollapsingHeader("Tesselation"))
	{
		ImGui::DragFloat("TessScale", (float *)&pBuffer->Data.heightScale, 0.001f, 0.0f, 0.1f);
	}

	ImGui::End();
}

void Tesselation::PreRender(ExecuteValues* exValue)
{
}

void Tesselation::CreateData()
{
	//mesh = (MeshData<VertexType> *)MeshManager::Get()->GetMesh(L"Quad");

	pBuffer = new TesselationBuffer();
	shader = ShaderManager::Get()->GetShader(Shaders + L"TesselationTestRender_tri.hlsl", ShaderType::VHDP);
}

void Tesselation::CreateTexture()
{
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_disp.png"));
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"toy_box_normal.png"));
	views.push_back(SRVManager::Get()->GetSRV(Textures + L"wood.png"));
}