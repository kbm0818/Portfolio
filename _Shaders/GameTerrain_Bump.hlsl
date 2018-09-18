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
	float4 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
	float4 depthPosition : TEXCOORD1;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

    output.position = mul(float4(input.position, 1.0f), _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

	output.uv = float4(input.uv, input.position.xz / 256.0f);

	output.normal = normalize(mul(input.normal, (float3x3)_world));
	output.tangent = normalize(mul(input.tangent, (float3x3)_world));
	output.binormal = normalize(mul(input.binormal, (float3x3)_world));

	output.depthPosition = output.position;

	return output;
}

Texture2D _map[5] : register(t0);
SamplerState _samp;

float4 PS(PixelInput input) : SV_TARGET
{
	float3x3 TBN = float3x3(normalize(input.tangent), normalize(input.binormal), normalize(input.normal));

	float4 textureColor;
	float4 grassColor = saturate(_map[0].Sample(_samp, input.uv.zw));
	float4 slopeColor = saturate(_map[1].Sample(_samp, input.uv.zw));
	float4 rockColor = saturate(_map[2].Sample(_samp, input.uv.zw));

	float slope = 1.0f - input.normal.y;
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	float3 bumpNormal;

	// 기본 텍스쳐를 기울기에 따라 선택

	if (slope < 0.2f)
	{
		textureColor = lerp(grassColor, slopeColor, slope / 0.2f);
	}
	if (slope < 0.7f && slope >= 0.2f)
	{
		textureColor = lerp(slopeColor, rockColor, (slope - 0.2f) * (1.0f / (0.7f - 0.2f)));
	}
	if (slope >= 0.7f)
	{
		textureColor = rockColor;
	}

	// 거리에따라 텍스쳐 하나를 합성

	if (depthValue < 0.995f)
	{ 
		float4 bumpMap = _map[4].Sample(_samp, input.uv.xy);
		bumpMap = (bumpMap * 2.0f) - 1.0f;
		bumpNormal = mul(bumpMap.xyz, TBN);

		textureColor = textureColor * _map[3].Sample(_samp, input.uv.xy) * 1.8f;
	}
	else
	{
		bumpNormal = normalize(input.normal);
	}

	return NormalColor(float4(0.1f, 0.1f, 0.1f, 1.0f), textureColor, _direction, bumpNormal);
}