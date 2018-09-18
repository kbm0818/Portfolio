#include "stdafx.h"
#include "AttachTarget.h"

AttachTarget::AttachTarget() : ComponentBase()
{
	target = nullptr;
}

AttachTarget::~AttachTarget()
{
}

void AttachTarget::Update()
{
	gameObject->SetPosition(target->GetWorldPosition());
}

//void SkyRenderer::Render()
//{
	//UINT stride = sizeof(VertexType);
	//UINT offset = 0;

	//D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//worldBuffer->SetVSBuffer(1);
	//skyBuffer->SetPSBuffer(1);
	//shader->Render();


	//States::SetDepthStencil(depthOffState);
	//States::SetRasterizer(countClockState);
	//{
	//	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	//}
	//States::SetRasterizer(clockState);
	//States::SetDepthStencil(depthOnState);
//}
//
//void SkyRenderer::PostRender()
//{
//	ImGui::Begin("Environment");
//
//	if (ImGui::CollapsingHeader("Sky"))
//	{
//		ImGui::ColorEdit3("Center", (float *)&skyBuffer->Data.Center);
//		ImGui::ColorEdit3("Apex", (float *)&skyBuffer->Data.Apex);
//	}
//
//	ImGui::End();
//}
