cbuffer VS_ViewProjection : register(b0) //cbuffer : RAM-> VRAM ���� �Ѿ �����ǰ� ������ �̰��̿�
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer �� ���
{
    matrix _world;
}

cbuffer VS_Brush : register(b2)
{
    uint _brushType;
    float3 _brushLocation;
    int _brushRange;
    float3 _brushColor;
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
    float3 brushColor : COLOR1;
};

SamplerState _sampler : register(s0);
Texture2D _map : register(t0); //�׻� ��ȣ ��ġ�ϰ�

float3 GetBrushColor(float3 location)
{
    if (_brushType == 0)
        return float3(0, 0, 0);

    if (_brushType == 1) //�簢�� �϶�
    {
        if ((location.x >= (_brushLocation.x - _brushRange)) &&
            (location.x <= (_brushLocation.x + _brushRange)) &&
            (location.z >= (_brushLocation.z - _brushRange)) &&
            (location.z <= (_brushLocation.z + _brushRange)))
        {
            return _brushColor;
        }
    }

    if (_brushType == 2)//��
    {
        float dx = location.x - _brushLocation.x;
        float dy = location.z - _brushLocation.z;

        float dist = sqrt(dx * dx + dy * dy);
        if (dist <= _brushRange)
            return _brushColor;
    }

    return float3(0, 0, 0);
}

PixelInput VS(VertexInput input) //IA ���� �ϷγѾ�� (IA -> VS) VS�� ������ ó��
{
    PixelInput output;
    output.brushColor = GetBrushColor(input.position.xyz); //�׳� ������ǥ�θ� �Ѱ���
    output.position = mul(input.position, _world); //����� �ٲ�
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.normal = mul(input.normal, (float3x3) _world);
    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET //������ ���ؽ����� ó���Ұ��̴ϱ� �Ϸγѱ� (VS -> PS) Rasterizing ���ؼ� �̹����� ��ȯ���� PS�� �Ѿ
{
    float3 light = _direction * -1;
    float intensity = saturate(dot(input.normal, light)); //saturate 0~1

    float4 brushColor = float4(input.brushColor, 0);
    float4 diffuseMap = _map.Sample(_sampler, input.uv) * intensity;
    return diffuseMap + brushColor;
}