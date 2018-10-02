#include "stdafx.h"
#include "RenderOptions.h"
#include "States.h"

RenderOptions::RenderOptions()
	:clockState(nullptr), countClockState(nullptr),
	ZBufferOnState(nullptr), ZBufferOffState(nullptr),
	alphaBlendEnableState(nullptr), alphaBlendDisableState(nullptr)
{
	states = new States;

	D3D11_RASTERIZER_DESC rasterizerDesc;
	states->GetRasterizerDesc(&rasterizerDesc);
	rasterizerDesc.FrontCounterClockwise = false;
	states->CreateRasterizer(&rasterizerDesc, &clockState);

	states->GetRasterizerDesc(&rasterizerDesc);
	rasterizerDesc.FrontCounterClockwise = true;
	states->CreateRasterizer(&rasterizerDesc, &countClockState);

	states->GetRasterizerDesc(&rasterizerDesc);
	rasterizerDesc.FrontCounterClockwise = true;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	states->CreateRasterizer(&rasterizerDesc, &allClockState);

	states->GetRasterizerDesc(&rasterizerDesc);
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	states->CreateRasterizer(&rasterizerDesc, &wireframeState);

	D3D11_DEPTH_STENCIL_DESC depthDesc;
	states->GetDepthStencilDesc(&depthDesc);
	depthDesc.DepthEnable = true;
	states->CreateDepthStencil(&depthDesc, &ZBufferOnState);

	states->GetDepthStencilDesc(&depthDesc);
	depthDesc.DepthEnable = false;
	states->CreateDepthStencil(&depthDesc, &ZBufferOffState);

	D3D11_BLEND_DESC blendDesc;
	states->GetBlendDesc(&blendDesc);
	states->CreateBlend(&blendDesc, &alphaBlendDisableState);

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	states->CreateBlend(&blendDesc, &alphaBlendEnableState);

	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	states->CreateBlend(&blendDesc, &otherBlendState);

	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	D3D::GetDevice()->CreateSamplerState(&samplerDesc, &wrap);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

	// Create the texture sampler state.
	D3D::GetDevice()->CreateSamplerState(&samplerDesc, &mirror);

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	// Create the texture sampler state.
	D3D::GetDevice()->CreateSamplerState(&samplerDesc, &clamp);
}

RenderOptions::~RenderOptions()
{
	SAFE_RELEASE(otherBlendState);
	SAFE_RELEASE(alphaBlendDisableState);
	SAFE_RELEASE(alphaBlendEnableState);

	SAFE_RELEASE(ZBufferOffState);
	SAFE_RELEASE(ZBufferOnState);

	SAFE_RELEASE(countClockState);
	SAFE_RELEASE(clockState);
	SAFE_RELEASE(allClockState);
	SAFE_RELEASE(wireframeState);

	SAFE_RELEASE(wrap);
	SAFE_RELEASE(mirror);
	SAFE_RELEASE(clamp);

	SAFE_DELETE(states);
}

void RenderOptions::TurnOnZBuffer()
{
	states->SetDepthStencil(ZBufferOnState);
}

void RenderOptions::TurnOffZBuffer()
{
	states->SetDepthStencil(ZBufferOffState);
}

void RenderOptions::TurnOnAlphaBlending()
{
	states->SetBlend(alphaBlendEnableState);
}

void RenderOptions::TurnOnOtherBlending()
{
	states->SetBlend(otherBlendState);
}

void RenderOptions::TurnOffAlphaBlending()
{
	states->SetBlend(alphaBlendDisableState);
}

void RenderOptions::TurnOnCullMode()
{
	states->SetRasterizer(clockState);

	vector<int> temp;
	int t;
	temp.push_back(t);
}

void RenderOptions::TurnOnCountCullMode()
{
	states->SetRasterizer(countClockState);
}

void RenderOptions::TurnOnAllCullMode()
{
	states->SetRasterizer(allClockState);
}

void RenderOptions::TurnOnWireframeMode()
{
	states->SetRasterizer(wireframeState);
}

void RenderOptions::SetSampler(SamplerType type, UINT slot)
{
	switch (type)
	{
	case SamplerType::WRAP:
		D3D::GetDC()->PSSetSamplers(slot, 1, &wrap);
		break;
	case SamplerType::CLAMP:
		D3D::GetDC()->PSSetSamplers(slot, 1, &clamp);
		break;
	case SamplerType::MIRROR:
		D3D::GetDC()->PSSetSamplers(slot, 1, &mirror);
		break;
	default:
		break;
	}
}
