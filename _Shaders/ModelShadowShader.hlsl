#include "Constants.hlsl"
#include "Utility.hlsl"

cbuffer VS_Bone: register(b3)
{
	float4x4 _boneArray[100];
	uint _skinning;
	float3 _bonePadding;
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

struct VertexInput
{
	float4 position : POSITION0;
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
	float4 lightViewPosition : TEXCOORD0;
	float2 uv : TEXCOORD1;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
	float3 lightPos : TEXCOORD2;
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
	output.lightPos = normalize(_cameraPosition - output.position);

	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.normal = normalize(mul(input.normal, (float3x3) world));
	output.tangent = normalize(mul(input.tangent, (float3x3) world));
	output.binormal = normalize(mul(input.binormal, (float3x3) world));

	output.uv = input.uv;

	output.lightViewPosition = mul(float4(input.position, 1.0f), world);
	output.lightViewPosition = mul(output.lightViewPosition, _lightView);
	output.lightViewPosition = mul(output.lightViewPosition, _lightProjection);

	return output;
}

Texture2D _map[5] : register(t0);
SamplerState _samp;
//0: ambient, 1: diffuse, 2:specular, 3.emissive, 4.normal

float4 PS(PixelInput input) : SV_TARGET
{
	float3 T = normalize(input.tangent);
	float3 B = normalize(input.binormal);
	float3 N = normalize(input.normal);

	float3x3 TBN = float3x3(T, B, N);

	float3 V = normalize(input.viewDirection);
	
	float4 diffuseColor = _map[1].Sample(_samp, input.uv);
	float3 normal = _map[4].Sample(_samp, input.uv).rgb;
	normal = 2 * normal - 1;
	normal = mul(normal, TBN);
	float4 specular = _map[2].Sample(_samp, input.uv);

	//return _diffuse;
	//return NormalColor(_ambient, diffuseColor, _light, normal);
	return SpecularNormalColor(_ambient, diffuseColor, specular, length(_specular), normal, V, _direction);
}