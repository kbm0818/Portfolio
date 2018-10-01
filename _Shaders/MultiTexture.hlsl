cbuffer VS_ViewProjection : register(b0) //cbuffer : RAM-> VRAM 으로 넘어감 임의의값 넣을때 이걸이용
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer 가 담당
{
    matrix _world;
}

cbuffer PS_Select : register(b0)
{
    uint _selected;
    float SelectPadding[3];
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

SamplerState _sampler : register(s0);
Texture2D _map[2] : register(t0); //항상 번호 일치하게

PixelInput VS(VertexInput input) //IA 에서 일로넘어옴 (IA -> VS) VS는 정점만 처리
{
    PixelInput output;

    output.position = mul(input.position, _world); //월드로 바꿈
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 color1 = _map[0].Sample(_sampler, input.uv);
    float4 color2 = _map[1].Sample(_sampler, input.uv);

    float4 diffuse = float4(0, 0, 0, 0);

    if (_selected == 0)
        diffuse = color1;
    if (_selected == 1)
        diffuse = color2;
    if (_selected == 2)
        diffuse = color1 * color2; //Modulate
    if (_selected == 3)
        diffuse = color1 * color2 * 2; //Modulate2X
    if (_selected == 4)
        diffuse = color1 *color2 * 4; //Modulate4X
    if (_selected == 5)
        diffuse = color1 + color2; //Add
    if (_selected == 6)
        diffuse = color1 + color2 - 0.5f; //Singed
    if (_selected == 7)
        diffuse = (color1 + color2 - 0.5f) * 2; //Singed2X
    if (_selected == 8)
        diffuse = color1 + color2 - color1 * color2; //Add Smooth
    if (_selected == 9)
        diffuse = color1 - color2; //Subtract
    if (_selected == 10)
        diffuse = color2 - color1; //InvSubtract
    if (_selected == 11)
        diffuse = 1.0f - color1; //Invsert color1
    if (_selected == 12)
        diffuse = 1.0f - color2; //Invsert color2
    if (_selected == 13)
        diffuse = 1.0f - (color1 + color2); //Invsert(color1 +color2)

    return diffuse;
}