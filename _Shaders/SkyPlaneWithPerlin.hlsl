#include "Constants.hlsl"

cbuffer PS_SkyPlaneBuffer : register(b1) // PS(1)
{
	float scale;
	float brightness;
	float translation;
	float padding;
}

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

	output.position = mul(float4(input.position,1.0f), _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.uv = input.uv;

	return output;
}

Texture2D _map[2] : register(t0);
SamplerState _samp;

float4 PS(PixelInput input) : SV_TARGET
{
	float4 perturbValue;
	float4 cloudColor;

	// Translate the texture coordinate sampling location by the translation value.
	input.uv.x = input.uv.x + translation;

	//// Sample the texture value from the perturb texture using the translated texture coordinates.
	perturbValue = _map[1].Sample(_samp, input.uv);

	//// Multiply the perturb value by the perturb scale.
	perturbValue = perturbValue * scale;
	
	// Add the texture coordinates as well as the translation value to get the perturbed texture coordinate sampling location.
	perturbValue.xy = perturbValue.xy + input.uv.xy + translation;

	//// Now sample the color from the cloud texture using the perturbed sampling coordinates.
	cloudColor = _map[0].Sample(_samp, perturbValue.xy);

	//// Reduce the color cloud by the brightness value.
	cloudColor = cloudColor * brightness;

	return cloudColor;
}