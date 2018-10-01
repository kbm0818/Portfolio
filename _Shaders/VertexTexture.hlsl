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
    float2 uv : TEXCOORD0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

PixelInput VS(VertexInput input) //IA 에서 일로넘어옴 (IA -> VS) VS는 정점만 처리
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;

    return output;
}

SamplerState _sampler : register(s0);
Texture2D _map : register(t0); //항상 번호 일치하게

float4 PS(PixelInput input) : SV_TARGET //색상은 버텍스에서 처리불가이니깐 일로넘김 (VS -> PS) Rasterizing 을해서 이미지로 변환된후 PS로 넘어감
{
    float4 color = _map.Sample(_sampler, input.uv);
    return color;
}