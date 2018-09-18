#include "stdafx.h"
#include "SkyShader.h"

SkyShader::SkyShader(wstring shaderFile) : Shader(shaderFile)
{
	skyBuffer = new SkyBuffer;
}

SkyShader::~SkyShader()
{
	SAFE_DELETE(skyBuffer);
}

void SkyShader::Render()
{
	skyBuffer->SetPSBuffer(1);
	Shader::Render();
}

void SkyShader::PostRender()
{
	ImGui::Begin("Environment");

	if (ImGui::CollapsingHeader("Sky"))
	{
		ImGui::ColorEdit3("Center", (float *)&skyBuffer->Data.Center);
		ImGui::ColorEdit3("Apex", (float *)&skyBuffer->Data.Apex);
	}

	ImGui::End();
}

void SkyShader::SetStates()
{
	States::SetDepthOff();
	States::SetRasterCountClock();
}
