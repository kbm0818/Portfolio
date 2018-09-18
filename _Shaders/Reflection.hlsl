#include "Constants.hlsl"

Texture2D _map[5] : register(t0);
SamplerState _samp;

cbuffer PS_TerrainBuffer :register(b1)
{
	float4 ambientColor;
	float4 diffuseColor;
};

cbuffer VS_ClipPlaneBuffer :register(b2)
{
	float4 clipPlane;
};

struct VertexInput
{
	float3 position : POSITION0;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
	float4 depthPosition : TEXCOORD1;
	float clip : SV_ClipDistance0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = mul(float4(input.position, 1.0f), _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.color = input.color;

	output.uv = input.uv;

	output.normal = normalize(mul(input.normal, (float3x3)_world));
	output.tangent = normalize(mul(input.tangent, (float3x3)_world));
	output.binormal = normalize(mul(input.binormal, (float3x3)_world));

	output.depthPosition = output.position;

	output.clip = dot(mul(float4(input.position, 1.0f), _world), clipPlane);

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 textureColor;
	float4 grassColor = saturate(_map[0].Sample(_samp, input.uv.zw));
	float4 slopeColor = saturate(_map[1].Sample(_samp, input.uv.zw));
	float4 rockColor = saturate(_map[2].Sample(_samp, input.uv.zw));
	float4 color = ambientColor;
	float3 lightDir = _direction * -1.0f;
	float lightIntensity;
	float slope = 1.0f - input.normal.y;
	float depthValue = input.depthPosition.z / input.depthPosition.w;
	float depthBright = 1.8f;
	float4 bumpMap;
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
		bumpMap = _map[4].Sample(_samp, input.uv.xy);
		bumpMap = (bumpMap * 2.0f) - 1.0f;
		bumpNormal = normalize(input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal);

		textureColor = textureColor * _map[3].Sample(_samp, input.uv.xy) * depthBright;
	}
	else
	{
		bumpNormal = input.normal;
	}

	lightIntensity = saturate(dot(bumpNormal, lightDir));

	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
	}

	return saturate(color) * textureColor;
}