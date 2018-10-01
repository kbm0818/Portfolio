#include "000_Header.hlsl"

Texture2D RenderTarget : register(t5);
SamplerState RenderTargetSample : register(s5);

struct PixelInput
{
    float4 position : SV_POSITION; //SV는 하드웨어로 직접들어감
    float2 uv : TEXCOORD0;
};

PixelInput VS(VertexTexture input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;
    //output.uv = float2(1 - input.uv.x, input.uv.y);

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return RenderTarget.Sample(RenderTargetSample, input.uv);
}