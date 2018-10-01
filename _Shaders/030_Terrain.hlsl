#include "000_Header.hlsl"

cbuffer PS_Grid : register(b2)
{
    float4 GridColor;
    uint GridSpacing;
    float GridThickness;

    uint GridView;
    float GridPadding;
};

struct PixelInput
{
    float4 position : SV_POSITION; //SV�� �ϵ����� ������
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 oPosition : POSITION0;
};

PixelInput VS(VertexTextureNormal input)
{
    PixelInput output;

    output.position = mul(input.position, _world); //���̴��� �ѱ涩 ���켱���� �Ѱܾ��ϰ� ����� ��켱���� ���
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.normal = GetWorldNormal(input.normal, _world);

    output.uv = input.uv;
    output.oPosition = input.position;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuseMap = _diffuseMap.Sample(_diffuseSampler, input.uv);
    float4 diffuse = GetDiffuseColor(_diffuse, _direction, input.normal);

    float4 color = diffuseMap * diffuse;

    if(GridView == 1)
    {
        float2 grid = input.oPosition.xz / GridSpacing;
    
        float2 range = abs(frac(grid - 0.5f) - 0.5f);
        float2 speed = fwidth(grid); //��ũ�� ��ǥ������ ��̺� abs(ddx(x) + ddy(x))

        float2 pixelRange = range / speed; //����ؼ� ĥ�ϱ⶧�� ���� ���������
        float lineWeight = saturate(min(pixelRange.x, pixelRange.y) - GridThickness);
        return lerp(GridColor, color, lineWeight);
    }

    return color;
}