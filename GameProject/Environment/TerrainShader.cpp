#include "stdafx.h"
#include "TerrainShader.h"

TerrainShader::TerrainShader(wstring shaderFile) : Shader(shaderFile)
{
	terrainBuffer = new TerrainBuffer;
}

TerrainShader::~TerrainShader()
{
	SAFE_DELETE(terrainBuffer);
}

void TerrainShader::Render()
{
	terrainBuffer->SetPSBuffer(1);
	Shader::Render();
}

void TerrainShader::PostRender()
{
	ImGui::Begin("Environment");

	if (ImGui::CollapsingHeader("Terrain"))
	{
		/*ImGui::ColorEdit3("Center", (float *)&skyBuffer->Data.Center);
		ImGui::ColorEdit3("Apex", (float *)&skyBuffer->Data.Apex);*/
	}

	ImGui::End();
}

void TerrainShader::SetStates()
{
	States::SetDepthOff();
	States::SetRasterCountClock();
}
