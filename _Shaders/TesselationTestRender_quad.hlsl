#include "Utility.hlsl"

cbuffer DS_ViewProjection : register(b0)
{
	matrix _view;
	matrix _projection;
	matrix _reflection;
	float3 _viewPosition;
	float vpPadding;
}

cbuffer DS_World : register(b1)
{
	matrix _world;
}

cbuffer PS_Sun : register(b0)
{
	float3 _direction;
	float _sunPadding;
}

cbuffer DS_TessBuffer : register(b2)
{
	float _heightScale;
	float3 _tessPadding;
}

/////////////////////////////////////////////////////////////////////////

Texture2D _map[3] : register(t0);
SamplerState _samp;

struct VertexIn
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

struct VertexOut
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	vout.position = vin.position;
	vout.uv = vin.uv;
	vout.normal = vin.normal;
	vout.tangent = vin.tangent;
	vout.binormal = vin.binormal;

	return vout;
}

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	pt.EdgeTess[0] = 64.0f;
	pt.EdgeTess[1] = 64.0f;
	pt.EdgeTess[2] = 64.0f;
	pt.EdgeTess[3] = 64.0f;

	pt.InsideTess[0] = 64.0f;
	pt.InsideTess[1] = 64.0f;

	return pt;
}

struct HullOut
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p,
	uint i : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID)
{
	HullOut hout;

	hout.position = p[i].position;
	hout.uv = p[i].uv;
	hout.normal = p[i].normal;
	hout.tangent = p[i].tangent;
	hout.binormal = p[i].binormal;

	return hout;
}

struct DomainOut
{
	float4 PosH : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : TANGENT0;
	float3 binormal : BINORMAL0;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("quad")]
DomainOut DS(PatchTess patchTess,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;

	// Bilinear interpolation.
	float3 v1 = lerp(quad[0].position, quad[1].position, uv.x);
	float3 v2 = lerp(quad[2].position, quad[3].position, uv.x);
	float3 p = lerp(v1, v2, uv.y);

	float2 t1 = lerp(quad[0].uv, quad[1].uv, uv.x);
	float2 t2 = lerp(quad[2].uv, quad[3].uv, uv.x);
	float2 t = lerp(t1, t2, uv.y);

	//// Displacement mapping
	p.z = -_map[0].SampleLevel(_samp, t, 0).r * _heightScale;
	
	dout.PosH = mul(float4(p, 1.0f), _world);
	dout.PosH = mul(dout.PosH, _view);
	dout.PosH = mul(dout.PosH, _projection);

	dout.uv = t;

	dout.normal = mul(quad[0].normal,(float3x3) _world);
	dout.tangent = mul(quad[0].tangent, (float3x3) _world);
	dout.binormal = mul(quad[0].binormal, (float3x3) _world);

	return dout;
}

float4 PS(DomainOut pin) : SV_Target
{
	float4 ambient = float4(0.1f,0.1f,0.1f,1.0f);
	float4 diffuse = _map[2].Sample(_samp, pin.uv);
	float3 lightDirection = _direction;
	
	float3 T = normalize(pin.tangent);
	float3 B = normalize(pin.binormal);
	float3 N = normalize(pin.normal);

	float3x3 TBN = float3x3(T, B, N);
	TBN = transpose(TBN);

	float3 normal = _map[1].Sample(_samp, pin.uv).xyz;
	normal = (normal * 2.0f) - 1.0f;
	normal = (normal.x * T) + (normal.y * B) + (normal.z * N);
	normal = normalize(normal);

	float4 color = NormalColor(ambient, diffuse, lightDirection, normal);

	//return color;
	return float4((pin.binormal * 2 - 1), 1.0f);
}