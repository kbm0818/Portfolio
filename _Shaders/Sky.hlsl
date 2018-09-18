#include "Constants.hlsl"

cbuffer PS_Sky : register(b1)
{
	float4 _center;
	float4 _apex;
}

struct VertexInput
{
	float3 position : POSITION0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float3 oPosition : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = mul(float4(input.position,1.0f), _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.oPosition = input.position;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 color = lerp(_center, _apex, input.oPosition.y);

	//float factor = dot(_direction, input.oPosition);

	//if (factor > 0.99995f)
	//color.xyz += float3(1.0f, 0.0f, 1.0f);

	return color;
}