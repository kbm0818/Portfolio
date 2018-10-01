cbuffer VS_ViewProjection : register(b0)
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1)
{
    matrix _world;
}

cbuffer PS_Light : register(b0)
{
    float3 _direction;
    float LightPadding;
}

cbuffer PS_Select : register(b1)
{
    float3 _LB;
    float3 _RT;
    float _GridSize;
    float Padding;
}
struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float3 normal : OUTPUT0;
    float4 selectPos : OUTPUT1;
};

SamplerState _sampler : register(s0);
Texture2D _map : register(t0);

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.selectPos = output.position;
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.normal = mul(input.normal, (float3x3) _world);

    output.uv = input.uv;
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float3 light = _direction * -1;
    float intensity = saturate(dot(input.normal, light)); 

    float4 color = float4(1, 1, 1, 1);
    float2 RtUv, LbUv;

    LbUv.x = _LB.x - _GridSize / 2.0f;
    LbUv.y = _LB.z - _GridSize / 2.0f;

    RtUv.x = LbUv.x +  _GridSize;
    RtUv.y = LbUv.y +  _GridSize;

    if (input.selectPos.x >= LbUv.x && input.selectPos.x <= RtUv.x
        && input.selectPos.z >= LbUv.y && input.selectPos.z <= RtUv.y)
        return float4(0, 1, 0, 1);

    return _map.Sample(_sampler, input.uv) * intensity;
}