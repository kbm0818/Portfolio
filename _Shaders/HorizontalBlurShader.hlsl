#include "Constants.hlsl"

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
	output.position = mul(output.position, _orthoView);
	output.position = mul(output.position, _orthoProjection);

	output.uv = input.uv;

	return output;
}

Texture2D _map;
SamplerState _samp;

cbuffer BlurBuffer : register(b2)
{
	float _screenheight;
	float _screenwidth;
	float2 blurPadding;
};

float4 PS(PixelInput input) : SV_TARGET
{
	const float offset[] = { 0.0, 1.0, 2.0, 3.0, 4.0 };
	const float weight[] = {
	0.2270270270, 0.1945945946, 0.1216216216,
	0.0540540541, 0.0162162162
	};

	float4 color = _map.Sample(_samp, input.uv) * weight[0];

	for (int i = 1; i < 5; i++)
	{
		color +=
			_map.Sample(_samp, input.uv + float2(0,offset[i]) / _screenheight) * weight[i] +
			_map.Sample(_samp, input.uv - float2(0,offset[i]) / _screenheight) * weight[i] ;
	}

	return color;
}