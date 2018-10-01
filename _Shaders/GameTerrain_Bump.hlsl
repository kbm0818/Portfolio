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
	float4 lightViewPosition : TEXCOORD2;
	float zFogFactor : FOG1;
	float yFogFactor : FOG2;
	float4 fogColor : FOG3;
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

	output.lightViewPosition = mul(float4(input.position, 1.0f), _world);
	output.lightViewPosition = mul(output.lightViewPosition, _lightView);
	output.lightViewPosition = mul(output.lightViewPosition, _lightProjection);

	float4 pos = mul(float4(input.position, 1.0f), _world);
	output.yFogFactor = saturate((_yFogEnd - pos.y) / (_yFogEnd - _yFogStart));

	pos = mul(pos, _view);
	output.zFogFactor = saturate((_zFogEnd - pos.z) / (_zFogEnd - _zFogStart));

	output.fogColor = _fogColor;
	
	return output;
}

Texture2D _depthMap : register(t0);
Texture2D _map[5] : register(t1);

SamplerState _sampWrap : register(s0);
SamplerState _sampClamp : register(s1);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 color = _ambientColor;
	float3 lightDir = -_direction;

	// 기본 텍스쳐를 기울기에 따라 선택 //////////////////////
	float4 textureColor;
	float4 grassColor = saturate(_map[0].Sample(_sampWrap, input.uv.zw));
	float4 slopeColor = saturate(_map[1].Sample(_sampWrap, input.uv.zw));
	float4 rockColor = saturate(_map[2].Sample(_sampWrap, input.uv.zw));
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
	//////////////////////////////////////////////////////////////////

	// 노멀 계산 /////////////////////////////////////////////////////
	float3 normal;
	float farValue = input.depthPosition.z / input.depthPosition.w;
	if (farValue < 0.995f)
	{
		float3x3 TBN = float3x3(normalize(input.tangent), normalize(input.binormal), normalize(input.normal));
		float4 bumpMap = _map[4].Sample(_sampWrap, input.uv.xy);
		bumpMap = (bumpMap * 2.0f) - 1.0f;
		normal = mul(bumpMap.xyz, TBN);

		textureColor = textureColor * _map[3].Sample(_sampWrap, input.uv.xy) * 1.8f;
	}
	else
	{
		normal = normalize(input.normal);
	}
	/////////////////////////////////////////////////////////////////

	// 안개로 텍스쳐 살짝 변경 ////////////////////////////////////////

	textureColor = input.zFogFactor * textureColor + (1.0f - input.zFogFactor) * input.fogColor;
	//textureColor = input.yFogFactor * textureColor + (1.0f - input.yFogFactor) * fogColor;
	
		
	/////////////////////////////////////////////////////////////////

	// 그림자 계산 && 음영 계산 ////////////////////////////////////////////////////
	float bias = 0.0001f;
	float2 projectTexCoord;
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
	float depthValue, lightDepthValue, lightIntensity;

	//DepthMap에서 보이는 범위 내에 있다면
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = _depthMap.Sample(_sampClamp, projectTexCoord).r;
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		lightDepthValue = lightDepthValue - bias;

		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(normal, lightDir));

			if (lightIntensity > 0.0f)
			{
				color += (_diffuseColor * lightIntensity);

				color = saturate(color);
			}
		}
	}
	else
	{
		lightIntensity = saturate(dot(normal, lightDir));

		if (lightIntensity > 0.0f)
		{
			color += (_diffuseColor * lightIntensity);

			color = saturate(color);
		}
	}
	/////////////////////////////////////////////////////////////////

	// 최종 합성
	color = color * textureColor;

	return color;
}