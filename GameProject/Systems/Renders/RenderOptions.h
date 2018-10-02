#pragma once

enum class SamplerType
{
	WRAP = 0,
	MIRROR,
	CLAMP,
};

class RenderOptions
{
public:
	RenderOptions();
	~RenderOptions();

	void TurnOnZBuffer();
	void TurnOffZBuffer();

	void TurnOnAlphaBlending();
	void TurnOnOtherBlending();
	void TurnOffAlphaBlending();

	void TurnOnCullMode();
	void TurnOnCountCullMode();
	void TurnOnAllCullMode();
	void TurnOnWireframeMode();

	void SetSampler(SamplerType type = SamplerType::WRAP, UINT slot = 0);

private:
	ID3D11RasterizerState* clockState;
	ID3D11RasterizerState* countClockState;
	ID3D11RasterizerState* allClockState;
	ID3D11RasterizerState* wireframeState;

	ID3D11DepthStencilState* ZBufferOnState;
	ID3D11DepthStencilState* ZBufferOffState;

	ID3D11BlendState *alphaBlendEnableState;
	ID3D11BlendState *alphaBlendDisableState;
	ID3D11BlendState *otherBlendState;

	ID3D11SamplerState* wrap;
	ID3D11SamplerState* mirror;
	ID3D11SamplerState* clamp;

	class States* states;
};