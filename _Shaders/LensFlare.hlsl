#include "Constants.hlsl"

Texture2D _lensFlareTexture : register(t0);
SamplerState _samp;

cbuffer LensFlareBuffer : register(b3) // VS(1)
{
	float4 flareColor;
	float2 posOffSet;
	float2 uvOffSet;
	float scale;
	float3 padding;
}

struct VertexInput
{
	float2 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 diff : COLOR0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position.xy = (input.position * scale) + posOffSet;
	output.position.z = 0.5f;
	output.position.w = 1.0f;

	output.uv.xy = input.uv.xy + uvOffSet;

	output.diff = flareColor;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 color = _lensFlareTexture.Sample(_samp, input.uv);
	color * input.diff;

	//return color;
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
}