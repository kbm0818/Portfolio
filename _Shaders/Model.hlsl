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
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
	float3 viewDirection : TEXCOORD1;
	float4 lightViewPosition : TEXCOORD2;
	float3 lightPos : TEXCOORD3;
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
	
	float4 pos = mul(float4(input.position, 1.0f), world);

	output.position = mul(pos, _view);
	output.position = mul(output.position, _projection);

	output.lightViewPosition = mul(pos, _lightView);
	output.lightViewPosition = mul(output.lightViewPosition, _lightProjection);

	output.viewDirection = normalize(_cameraPosition - pos.xyz);
	output.lightPos = normalize(_lightPosition - pos.xyz);

	output.normal = normalize(mul(input.normal, (float3x3) world));
	output.tangent = normalize(mul(input.tangent, (float3x3) world));
	output.binormal = normalize(mul(input.binormal, (float3x3) world));

	output.uv = input.uv;

	return output;
}

cbuffer PS_Material : register(b1)
{
	float4 _ambient;
	float4 _diffuse;
	float4 _specular;
	float4 _emissive;
	float4 _normal;
	float _shininess;
	float3 _materialPadding;
}


Texture2D _depthMap : register(t0);
Texture2D _map[5] : register(t1);
//0: ambient, 1: diffuse, 2:specular, 3.emissive, 4.normal

SamplerState _sampWrap : register(s0);
SamplerState _sampClamp : register(s1);

float4 PS(PixelInput input) : SV_TARGET
{
	float4 color = _ambientColor;
	float3 lightDir = -_direction;

	// 노멀계산 ////////////////////////////////////////////////////////
	float3x3 TBN = float3x3(normalize(input.tangent), 
		normalize(input.binormal), 
		normalize(input.normal)
		);
	float3 normal = _map[4].Sample(_sampWrap, input.uv).rgb;
	normal = 2 * normal - 1;
	normal = mul(normal, TBN);
	//////////////////////////////////////////////////////////////////////////

	// 그림자 계산 && 음영 계산 ////////////////////////////////////////////////////
	float2 projectTexCoord;
	float bias = 0.0001f;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 specular = float4(_map[2].Sample(_sampWrap, input.uv).rgb, 1.0f);
	float3 V = normalize(input.viewDirection);

	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;

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
				float3 reflection = normalize(2 * lightIntensity * normal - lightDir);
				
				float4 specularIntensity = pow(saturate(dot(reflection, V)), _shininess);

				color += (_diffuseColor * lightIntensity) + (specularIntensity * specular);
				color = saturate(color);
			}
		}
	}
	else
	{
		lightIntensity = saturate(dot(normal, lightDir));

		if (lightIntensity > 0.0f)
		{
			float3 reflection = normalize(2 * lightIntensity * normal - lightDir);
			float4 specularIntensity = pow(saturate(dot(reflection, V)), _shininess);

			color += (_diffuseColor * lightIntensity) + (specularIntensity * specular);
			color = saturate(color);
		}
	}
	////////////////////////////////////////////////////////////////////////////


	/*lightIntensity = saturate(dot(normal, lightDir));
	if (lightIntensity > 0.0f)
	{
		color += (_diffuseColor * lightIntensity);
		color = saturate(color);
	}*/

	float4 textureColor = _map[1].Sample(_sampWrap, input.uv);

	color = color * textureColor;

	return color;
}