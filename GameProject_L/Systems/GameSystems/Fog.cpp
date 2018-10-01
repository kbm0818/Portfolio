#include "stdafx.h"
#include "Fog.h"

Fog::Fog()
{
	buffer = new FogBuffer;
}

Fog::~Fog()
{
	SAFE_DELETE(buffer);
}

void Fog::Render()
{
	buffer->SetVSBuffer(9);
}

void Fog::PostRender()
{
	ImGui::Begin("Fog");

	ImGui::DragFloat("zFogStart", &buffer->data.zStart);
	ImGui::DragFloat("zFogEnd", &buffer->data.zEnd);
	ImGui::DragFloat("yFogStart", &buffer->data.yStart);
	ImGui::DragFloat("yFogEnd", &buffer->data.yEnd);

	ImGui::Separator();
	ImGui::End();
}

void Fog::SetData(float zStart, float zEnd, float yStart, float yEnd)
{
	SetZData(zStart, zEnd);
	SetYData(yStart, yEnd);
}

void Fog::SetZData(float zStart, float zEnd)
{
	buffer->SetZFog(zStart, zEnd);
}

void Fog::SetYData(float yStart, float yEnd)
{
	buffer->SetYFog(yStart, yEnd);
}
