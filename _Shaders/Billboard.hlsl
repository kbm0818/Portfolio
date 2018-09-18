#include "Constants.hlsl"

Texture2D _map : register(t0);
SamplerState _samp;

struct VertexInput
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = mul(float4(input.position, 1.0f), _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.uv = input.uv;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	return _map.Sample(_samp, input.uv);
}