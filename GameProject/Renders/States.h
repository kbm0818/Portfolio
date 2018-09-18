#pragma once

class States
{
public:
	static void Create();
	static void Delete();

	static void GetRasterizerDesc(D3D11_RASTERIZER_DESC* desc);
	static void CreateRasterizer(D3D11_RASTERIZER_DESC* desc, ID3D11RasterizerState** state);
	static void SetRasterizer(ID3D11RasterizerState* state);

	static void GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* desc);
	static void CreateDepthStencil(D3D11_DEPTH_STENCIL_DESC* desc, ID3D11DepthStencilState** state);
	static void SetDepthStencil(ID3D11DepthStencilState* state);

	static void GetSamplerDesc(D3D11_SAMPLER_DESC* desc);
	static void CreateSampler(D3D11_SAMPLER_DESC* desc, ID3D11SamplerState** state);
	static void SetSampler(UINT slot, UINT count, ID3D11SamplerState* state);

	static void SetDepthOff();
	static void SetDepthDefault();
	static void SetSamplerDefault(UINT slot, UINT count);
	static void SetRasterClock();
	static void SetRasterCountClock();
	static void SetRasterOffCullMode();

private:
	static void CreateRasterizerDesc();
	static void CreateDepthStencilDesc();
	static void CreateSamplerDesc();

	static D3D11_RASTERIZER_DESC rasterizerDesc;
	static D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	static D3D11_SAMPLER_DESC samplerDesc;

	static ID3D11SamplerState* samplerDefault;

	static ID3D11DepthStencilState* depthDefault;
	static ID3D11DepthStencilState* depthOff;

	static ID3D11RasterizerState* rasterClock; // 시계방향
	static ID3D11RasterizerState* rasterCountClock; // 반시계방향
	static ID3D11RasterizerState* rasterOffCullMode; // 컬모드 끄기
};