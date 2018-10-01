cbuffer VS_ViewProjection : register(b0) //cbuffer : RAM-> VRAM ���� �Ѿ �����ǰ� ������ �̰��̿�
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer �� ���
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
    float3 wPosition : OUTPUT0;
};

SamplerState _sampler : register(s0);
Texture2D _map : register(t0); //�׻� ��ȣ ��ġ�ϰ�

PixelInput VS(VertexInput input) //IA ���� �ϷγѾ�� (IA -> VS) VS�� ������ ó��
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;
    output.wPosition = mul(input.position, _world);
    return output;
}

float4 PS(PixelInput input) : SV_TARGET //������ ���ؽ����� ó���Ұ��̴ϱ� �Ϸγѱ� (VS -> PS) Rasterizing ���ؼ� �̹����� ��ȯ���� PS�� �Ѿ
{
    float4 color = float4(1, 1, 1, 1);

    if (input.wPosition.y > 10)
        color = float4(1, 0, 1, 1);

    if (input.wPosition.y > 40)
        color = float4(0, 0, 1, 1);

    if (input.wPosition.y > 70)
        color = float4(0, 1, 0, 1);

    if(input.wPosition.y >100)
        color = float4(1, 0, 0, 1);

    return color;



   //return _map.Sample(_sampler, input.uv);
}