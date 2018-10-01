#include "Constants.hlsl"
#include "Utility.hlsl"

struct VertexInput
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

    output.position = mul(float4(input.position, 1.0f), _world);
    output.position = mul(output.position, _lightView);
    output.position = mul(output.position, _lightProjection);

	output.depthPosition = output.position;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float depthValue;
	float4 color;

	depthValue = input.depthPosition.z / input.depthPosition.w;
	color = float4(depthValue, depthValue, depthValue, 1.0f);

	return color;
}