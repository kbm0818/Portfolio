#include "stdafx.h"

#include "Object/Shader/Shader.h"
#include "ModelMaterial.h"
#include "ModelBuffer.h"
#include "Object\SRV\SRVFactory.h"
#include "Object\Shader\ShaderFactory.h"

ModelMaterial::ModelMaterial()
	: number(-1), shader(NULL)
{
	shaderBuffer = new MaterialBuffer();
}

ModelMaterial::~ModelMaterial()
{
	SAFE_DELETE(shaderBuffer);

	for (ID3D11ShaderResourceView* v : views)
		SAFE_RELEASE(v);
}

void ModelMaterial::SetPSBuffer(UINT slot)
{
	shader->Render(ShaderType::VP);
	shaderBuffer->SetPSBuffer(slot);
	values->RenderOption->SetSampler();
	D3D::GetDC()->PSSetShaderResources(0, (UINT)views.size(), &views[0]);
}

void ModelMaterial::Load(BinaryReader * r, GlobalValues* values)
{
	this->values = values;

	number = r->UInt();
	name = String::WStringToString(r->Wstring());

	shaderBuffer->SetAmbient(r->Color4f());
	shaderBuffer->SetDiffuse(r->Color4f());
	shaderBuffer->SetSpecular(r->Color4f());
	shaderBuffer->SetEmissive(r->Color4f());
	shaderBuffer->SetNormal(r->Color4f());
	shaderBuffer->SetShininess(r->Float());

	views.resize(5);
	views[0] = values->SRVFactory->Create(r->Wstring());
	views[1] = values->SRVFactory->Create(r->Wstring());
	views[2] = values->SRVFactory->Create(r->Wstring());
	views[3] = values->SRVFactory->Create(r->Wstring());
	views[4] = values->SRVFactory->Create(r->Wstring());

	shader = values->ShaderFactory->Create(Shaders + L"Model.hlsl", ShaderType::VP);
}
