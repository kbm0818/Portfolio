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

    return output;
}



float4 PS(PixelInput input) : SV_TARGET //������ ���ؽ����� ó���Ұ��̴ϱ� �Ϸγѱ� (VS -> PS) Rasterizing ���ؼ� �̹����� ��ȯ���� PS�� �Ѿ
{
    return _map.Sample(_sampler, input.uv);
}