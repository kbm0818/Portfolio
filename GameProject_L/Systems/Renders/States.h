#pragma once

class States
{
public:
	States();
	~States();

	void GetRasterizerDesc(D3D11_RASTERIZER_DESC* desc);
	void CreateRasterizer(D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state);
	void SetRasterizer(ID3D11RasterizerState* state);

	void GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc);
	void CreateDepthStencil(D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state);
	void SetDepthStencil(ID3D11DepthStencilState* state);

	void GetSamplerDesc(D3D11_SAMPLER_DESC* desc);
	void CreateSampler(D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state);
	void SetSampler(UINT slot, UINT count, ID3D11SamplerState* state);

	void GetBlendDesc(D3D11_BLEND_DESC* desc);
	void CreateBlend(D3D11_BLEND_DESC* desc, ID3D11BlendState** state);
	void SetBlend(ID3D11BlendState * state, UINT mask = 0xffffffff, float* factor = NULL);

private:
	void CreateRasterizerDesc();
	void CreateDepthStencilDesc();
	void CreateSamplerDesc();
	void CreateBlendDesc();

	D3D11_RASTERIZER_DESC rasterizerDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BLEND_DESC blendDesc;
};