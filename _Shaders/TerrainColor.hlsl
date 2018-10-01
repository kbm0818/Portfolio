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
    float4 color : COLOR0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

SamplerState _sampler : register(s0);
Texture2D _map : register(t0); //�׻� ��ȣ ��ġ�ϰ�

PixelInput VS(VertexInput input) //IA ���� �ϷγѾ�� (IA -> VS) VS�� ������ ó��
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