#include "000_Header.hlsl"

struct PixelInput
{
    float4 position : SV_POSITION; 
    float2 uv : UV0; //예약어 제외하고는 맘대로가능
};

PixelInput VS(VertexTexture input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuse = _diffuseMap.Sample(_diffuseSampler, input.uv);

    //if(diffuse.a < 0.5f)
    //    discard; //discard 는 바로 리턴됨
    
    clip(diffuse.a - 0.5f); //clip은 리턴은안됨


    return diffuse;
}