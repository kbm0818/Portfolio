#include "Constants.hlsl"
#include "Utility.hlsl"

cbuffer VS_Bone: register(b3)
{
	float4x4 _boneArray[100];
	uint _skinning;
	float3 _bonePadding;
}

struct VertexInput
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
	float4 boneIndices : BLENDINDICES0;
	float4 boneWeights : BLENDWEIGHT0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;
	float4x4 world = 0;

	if (_skinning == 0)
	{
		world = _world;
	}
	else
	{
		world += _boneArray[input.boneIndices.x] * input.boneWeights.x;
		world += _boneArray[input.boneIndices.y] * input.boneWeights.y;
		world += _boneArray[input.boneIndices.z] * input.boneWeights.z;
		world += _boneArray[input.boneIndices.w] * input.boneWeights.w;

		world = mul(world, _world);
	}

	output.position = mul(float4(input.position,1.0f), world);
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