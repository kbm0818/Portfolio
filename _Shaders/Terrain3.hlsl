cbuffer VS_ViewProjection : register(b0) //cbuffer : RAM-> VRAM 으로 넘어감 임의의값 넣을때 이걸이용
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer 가 담당
{
    matrix _world;
}

cbuffer PS_Light : register(b0)
{
    float3 _direction;
    float LightPadding;
}
struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float3 normal : OUTPUT0;
};

SamplerState _sampler : register(s0);
Texture2D _map : register(t0); //항상 번호 일치하게

PixelInput VS(VertexInput input) //IA 에서 일로넘어옴 (IA -> VS) VS는 정점만 처리
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.normal = mul(input.normal, (float3x3) _world);

    output.uv = input.uv;
    float height = input.position.y;
    float4 color = float4(1, 1, 1, 1);

    if (height > 5)
        color = float4(1, 0, 0, 1);

    if (height > 10)
        color = float4(0, 0, 1, 1);

    if (height > 15)
        color = float4(0, 1, 0, 1);

    if (height > 20)
        color = float4(1, 1, 0, 1);

    if (height > 25)
        color = float4(1, 0, 1, 1);

    if (height > 30)
        color = float4(0, 1, 1, 1);
    output.color = color;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET //색상은 버텍스에서 처리불가이니깐 일로넘김 (VS -> PS) Rasterizing 을해서 이미지로 변환된후 PS로 넘어감
{
    float3 light = _direction * -1;
    float intensity = saturate(dot(input.normal, light)); //saturate 0~1
    return _map.Sample(_sampler, input.uv) * intensity;
   //return _map.Sample(_sampler, input.uv);
}