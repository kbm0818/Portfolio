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
    float4 position : SV_POSITION;
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

    //if(GridView == 1)
    //{
    //    float2 grid = float2(0, 0);
    //    grid.x = frac(input.oPosition.x / (float) GridSpacing); //frac �Ҽ����� ��
    //    grid.y = frac(input.oPosition.z / (float) GridSpacing);

    //    if(grid.x < GridThickness || grid.y < GridThickness)
    //        return GridColor;
    //}

    //return diffuseMap * diffuse;

    float2 grid = input.oPosition.xz / GridSpacing;
    
    float2 range = abs(frac(grid - 0.5f) - 0.5f);
    float2 speed = fwidth(grid);
    //fwidth x �Ű� ������ ��̺� ���� ���Դϴ�. This function computes the following: abs(ddx(x)) + abs(ddy(x)).
    //ddx(x) Returns the partial derivative of the specified value with respect to the screen-space x-coordinate.
    //x��ǥ ���� ����� �ϴ°�
    float2 pixelRange = range / speed;
    float lineWeight = saturate(min(pixelRange.x, pixelRange.y) - GridThickness);

    return lerp(GridColor, diffuseMap * diffuse, lineWeight);
}