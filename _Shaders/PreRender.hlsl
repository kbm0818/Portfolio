cbuffer VS_Matrix : register(b0)
{
    matrix _world;
    matrix _view;
    matrix _projection;
}

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
     
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.uv       = input.uv;
    
    return output;
}

SamplerState _samp : register(s0);
Texture2D _map : register(t0);

float4 PS(PixelInput input) : SV_TARGET
{   
    return _map.Sample(_samp, input.uv);
}

