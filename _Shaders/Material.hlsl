#include "000_Header.hlsl"

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

SamplerState _sampler : register(s0);


PixelInput VS(VertexTextureNormal input)
{
    PixelInput output;

    output.position = mul(input.position, _world); //월드로 바꿈, TransformCoord와 같음 4x4라서 쉐이더는 열우선임 Dx10부터.
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.normal = GetWorldNormal(input.normal, _world);
    output.uv = input.uv;


    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuseMap = _diffuseMap.Sample(_sampler, input.uv);
    float4 diffuse = GetDiffuseColor(_diffuse, _direction, input.normal);

    return diffuseMap * diffuse;
}