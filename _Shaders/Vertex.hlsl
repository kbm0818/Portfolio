cbuffer VS_ViewProjection : register(b0) //cbuffer : RAM-> VRAM 으로 넘어감 임의의값 넣을때 이걸이용
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer 가 담당
{
    matrix _world;
}

cbuffer PS_Color : register(b0) //cpu, gpu 안에 고속메모리(register) 가들어있음 VRAM-> register 로 넘김 b0 버퍼번호
{
    float4 _color;
    uint _check;

    float3 _ColorPadding;
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

PixelInput VS(VertexInput input) //IA 에서 일로넘어옴 (IA -> VS) VS는 정점만 처리
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.color = input.color;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET //색상은 버텍스에서 처리불가이니깐 일로넘김 (VS -> PS) Rasterizing 을해서 이미지로 변환된후 PS로 넘어감
{
    float4 color;

    if (_check == 0)
        color = input.color;
	else
        color = _color;

    return color;
}