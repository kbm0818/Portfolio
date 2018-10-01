cbuffer VS_ViewProjection : register(b0) //cbuffer : RAM-> VRAM 으로 넘어감 임의의값 넣을때 이걸이용
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer 가 담당
{
    matrix _world;
}

struct VertexInput
{
    float4 position : POSITION0;
    float4 color : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

SamplerState _sampler : register(s0);
Texture2D _map : register(t0); //항상 번호 일치하게

PixelInput VS(VertexInput input) //IA 에서 일로넘어옴 (IA -> VS) VS는 정점만 처리
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.color = input.color;
    return output;
}

float4 PS(PixelInput input) : SV_TARGET 
{
    return input.color;
   //return _map.Sample(_sampler, input.uv);
}