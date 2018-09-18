#include "Constants.hlsl"
//Texture2D _DSmap[3] : register(t0);
//// 0:heightMap 1:normalMap 2:blendMap
//Texture2D _PSmap[5] : register(t0); 
//// 0: diffuseTexture 1~4:blendTexture
//SamplerState _samp;

struct VertexIn
{
	float4 position    : POSITION0; 
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

struct VertexOut
{
	float4 position    : SV_POSITION;
	float4 color : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vin.position.w = 1.0f;

	vout.position = mul(vin.position, _world);
	vout.position = mul(vout.position, _view);
	vout.position = mul(vout.position, _projection);

	vout.normal = normalize(mul(vin.normal, (float3x3)_world));

	vout.uv = vin.uv;
	vout.color = vin.color;

	return vout;
}

float4 PS(VertexOut vin) : SV_Target
{
	return float4(0.0f, 0.0f, 0.0f, 0.0f);
	//float4 diffuseMap = saturate(_PSmap[0].SampleLevel(_samp, pin.uv, 0));
	//float4 c1 = saturate(_PSmap[1].SampleLevel(_samp, pin.uv, 0));
	//float4 c2 = saturate(_PSmap[2].SampleLevel(_samp, pin.uv, 0));
	//float4 c3 = saturate(_PSmap[3].SampleLevel(_samp, pin.uv, 0));
	//float4 c4 = saturate(_PSmap[4].SampleLevel(_samp, pin.uv, 0));

	//float4 color = lerp(diffuseMap, c1, pin.blend.x);
	//color = lerp(color, c2, pin.blend.y);
	//color = lerp(color, c3, pin.blend.z);
	//color = lerp(color, c4, pin.blend.w);

	//float4 result = float4(0.0f,0.0f,0.0f,1.0f);

	//float3 light = _direction * -1;

	//float intensity = saturate(dot(pin.normal, light));

	//if (intensity > 0.0f)
	//	result += float4(1.0f, 1.0f, 1.0f, 1.0f) * intensity;

	////return saturate(result) * color;
	//return float4(pin.normal, 1.0f);
}