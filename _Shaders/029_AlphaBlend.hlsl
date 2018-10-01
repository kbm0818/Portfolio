#include "000_Header.hlsl"

cbuffer VS_Alpha : register(b2)
{
    matrix View;
    matrix Projection;
}

struct PixelInput
{
    float4 position : SV_POSITION; 
    float2 uv : UV0;
};

PixelInput VS(VertexTexture input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);

    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return _diffuseMap.Sample(_diffuseSampler, input.uv);
}