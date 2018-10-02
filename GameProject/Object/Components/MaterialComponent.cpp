#include "stdafx.h"
#include "MaterialComponent.h"

#include "../Shader/Shader.h"

MaterialComponent::MaterialComponent()
	: shader(nullptr), samplerType(SamplerType::WRAP)
{
	materialBuffer = new MaterialBuffer();
}

MaterialComponent::~MaterialComponent()
{
	SAFE_DELETE(materialBuffer);
}

void MaterialComponent::Update(GlobalValues * exValue)
{
}

void MaterialComponent::Render(GlobalValues * exValue)
{
	if(materialBuffer)
		materialBuffer->SetPSBuffer(1);

	if(views.size() > 0)
		D3D::GetDC()->PSSetShaderResources(0, (UINT)views.size(), &views[0]);

	exValue->RenderOption->SetSampler(samplerType, 0);

	if (shader)
		shader->Render(shaderType);
}

void MaterialComponent::PreRender(GlobalValues * exValue)
{
}

void MaterialComponent::PostRender(GlobalValues * exValue)
{
}

void MaterialComponent::AddSRV(ID3D11ShaderResourceView * srv)
{
	views.push_back(srv);
}

void MaterialComponent::AddColor(D3DXCOLOR color)
{
	materialBuffer->AddColor(color);
}

void MaterialComponent::SetShader(Shader * shader, ShaderType type)
{
	this->shader = shader;
	this->shaderType = type;
}
