cbuffer VS_ViewProjection : register(b0) //cbuffer : RAM-> VRAM ���� �Ѿ �����ǰ� ������ �̰��̿�
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer �� ���
{
    matrix _world;
}

cbuffer PS_Color : register(b0) //cpu, gpu �ȿ� ��Ӹ޸�(register) ��������� VRAM-> register �� �ѱ� b0 ���۹�ȣ
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

PixelInput VS(VertexInput input) //IA ���� �ϷγѾ�� (IA -> VS) VS�� ������ ó��
{
    PixelInput output;
    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.color = input.color;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET //������ ���ؽ����� ó���Ұ��̴ϱ� �Ϸγѱ� (VS -> PS) Rasterizing ���ؼ� �̹����� ��ȯ���� PS�� �Ѿ
{
    float4 color;

    if (_check == 0)
        color = input.color;
	else
        color = _color;

    return color;
}