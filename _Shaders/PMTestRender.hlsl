#include "Constants.hlsl"
#include "Utility.hlsl"

Texture2D _map[3] : register(t0);
SamplerState _samp;

cbuffer PS_POMBuffer : register(b1)
{
	float heightScale;
	float3 _pomPadding;
}

cbuffer VS_WorldInverseT : register(b2)
{
	matrix _worldIT;
}

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
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
	float3 viewDirection : TEXCOORD1;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = mul(float4(input.position, 1.0f), _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.uv = input.uv;

	output.normal = mul(input.normal, (float3x3)_world);
	output.tangent = mul(input.tangent, (float3x3)_world);
	output.binormal = mul(input.binormal, (float3x3)_world);

	float3 wPosition = mul(float4(input.position, 1.0f), _world).xyz;

	output.viewDirection = _viewPosition - wPosition;

	return output; 
}

float4 PS(PixelInput input) : SV_TARGET
{
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);

	float3x3 TBN = float3x3(T, B, N);
	TBN = transpose(TBN);

	float3 viewDirection = normalize(input.viewDirection);
	float3 view = mul(-viewDirection,TBN);
	
	////UV¼öÁ¤**************************************
	//ParallaxMapping
	float height = _map[0].Sample(_samp, input.uv).x;
	float2 vFinalCoords = parallaxMapping(view, input.uv, height, heightScale);
	
	float4 color = _map[2].Sample(_samp, vFinalCoords);

	float3 normal = _map[1].Sample(_samp, vFinalCoords).xyz;
	normal = 2.0f * normal - 1.0f;
	normal = mul(normal, TBN);

	return NormalColor(float4(0.2f,0.2f,0.2f,1.0f), color, _direction, normal);
	//return float4(normalize(input.viewDirection), 1.0f);
}