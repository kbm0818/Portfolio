#include "000_Header.hlsl"

cbuffer VS_ScalePosition : register(b2)
{
    float2 Scale;
    float2 Padding1;

    float3 Position;
    float RotationY;
}

struct PixelInput
{
    float4 position : SV_POSITION; 
    float2 uv : UV0; //예약어 제외하고는 맘대로가능
};

PixelInput VS(VertexTexture input)
{
    PixelInput output;

    matrix S, R, R2, T, srtWorld;
    S = R = R2 = T = srtWorld = matrix(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
    S = GetScale(S, float3(Scale.x, Scale.y, 1));

    R = _viewInverse;
    R._41 = R._42 = R._43 = 0.0f;
    R._12 = R._32 = 0.0f;
    R._21 = R._23 = 0.0f;

    R2._11_33 = float2(cos(RotationY), cos(RotationY));
    R2._13_31 = float2(sin(RotationY), -sin(RotationY));
    R = mul(R, R2);
    T = GetPosition(T, Position);
    
    srtWorld = mul(S, R);
    srtWorld = mul(srtWorld, T);
    output.position = mul(input.position, srtWorld);

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