#include "Constants.hlsl"

Texture2D _map[4] : register(t0);
SamplerState _samp;

cbuffer PS_TerrainBuffer :register(b1)
{
	float4 ambientColor;
	float4 diffuseColor;
};

struct VertexInput
{
	float3 position : POSITION0;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float4 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float4 depthPosition : TEXCOORD1;
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

	output.depthPosition = output.position;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 textureColor;
	float4 grassColor = saturate(_map[0].Sample(_samp, input.uv.xy));
	float4 slopeColor = saturate(_map[1].Sample(_samp, input.uv.xy));
	float4 rockColor = saturate(_map[2].Sample(_samp, input.uv.xy));

	float slope = 1.0f - input.normal.y;

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

	float depthValue = input.depthPosition.z / input.depthPosition.w;
	float depthBright = 1.8f;
	if (depthValue < 0.995f)
	{ 
		textureColor = textureColor * _map[3].Sample(_samp, input.uv.zw) * depthBright;
	}
	float4 color = ambientColor;
	float3 lightDir = _direction * -1.0f;
	float lightIntensity = saturate(dot(input.normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		color += (diffuseColor * lightIntensity);
	}

	color = saturate(color);

    return color * textureColor;
}