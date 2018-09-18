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
	float EdgeTess[3]   : SV_TessFactor;
	float InsideTess : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 3> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;

	pt.EdgeTess[0] = 64.0f;
	pt.EdgeTess[1] = 64.0f;
	pt.EdgeTess[2] = 64.0f;
	
	pt.InsideTess = 64.0f;
	
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

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 3> p,
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
[domain("tri")]
DomainOut DS(PatchTess patchTess,
	float3 baryCoords : SV_DomainLocation,
	const OutputPatch<HullOut, 3> tri)
{
	DomainOut dout;

	float3 p = baryCoords.x*tri[0].position +
		baryCoords.y*tri[1].position +
		baryCoords.z*tri[2].position;

	float2 t = baryCoords.x*tri[0].uv +
		baryCoords.y*tri[1].uv +
		baryCoords.z*tri[2].uv;
	
	// Displacement mapping
	p.z = -_map[0].SampleLevel(_samp, t, 0).r * _heightScale;
	
	dout.PosH = mul(float4(p, 1.0f), _world);
	dout.PosH = mul(dout.PosH, _view);
	dout.PosH = mul(dout.PosH, _projection);

	dout.uv = t;

	float3 T = baryCoords.x*tri[0].tangent +
		baryCoords.y*tri[1].tangent +
		baryCoords.z*tri[2].tangent;
	dout.tangent = mul(T, (float3x3)_world);

	float3 B = baryCoords.x*tri[0].binormal +
		baryCoords.y*tri[1].binormal +
		baryCoords.z*tri[2].binormal;
	dout.binormal = mul(B, (float3x3)_world);

	float3 N = baryCoords.x*tri[0].normal +
		baryCoords.y*tri[1].normal +
		baryCoords.z*tri[2].normal;
	dout.normal = mul(N, (float3x3)_world);

	return dout;
}

float4 PS(DomainOut pin) : SV_Target
{
	float4 ambient = float4(0.2f,0.2f,0.2f,1.0f);
	float4 diffuse = _map[2].Sample(_samp, pin.uv);
	float3 lightDirection = _direction;
	
	float3 T = normalize(pin.tangent);
	float3 B = normalize(pin.binormal);
	float3 N = normalize(pin.normal);

	float3x3 TBN = float3x3(T, B, N);
	TBN = transpose(TBN);

	float3 normal = _map[1].Sample(_samp, pin.uv).xyz;
	normal = (normal * 2.0f) - 1.0f;
	normal = mul(normal, TBN);

	return NormalColor(ambient, diffuse, _direction, normal);
}