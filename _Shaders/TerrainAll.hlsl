#include "000_Header.hlsl"

Texture2D _splatingMap[4] : register(t5);

cbuffer PS_Grid : register(b2)
{
    float4 GridColor;
    uint GridSpacing;
    float GridThickness;

    uint GridView;
    float GridPadding;
};

cbuffer VS_Brush : register(b2)
{
    uint BrushType;
    float3 BrushLocation;
    int BrushRange;
    float3 BrushColor;
}

cbuffer VS_Terrain : register(b3)
{
    float3 StartPos;
    float StartPadding;
    float3 EndPos;
    float EndPadding;

    float2 TerrainSize;
    float2 TerrainPadding;
    uint VertexGridSpacing;
    float3 VertexGridSpacingPadding;
}

float3 GetBrushColor(float3 location)
{
    if (BrushType == -1)
        return float3(0, 0, 0);

    if (BrushType == 0)//원
    {
        float dx = location.x - BrushLocation.x;
        float dy = location.z - BrushLocation.z;

        float dist = sqrt(dx * dx + dy * dy);
        if (dist <= BrushRange)
            return BrushColor;
    }

    if (BrushType == 1) //사각형 일때
    {
        if ((location.x >= (BrushLocation.x - BrushRange)) &&
            (location.x <= (BrushLocation.x + BrushRange)) &&
            (location.z >= (BrushLocation.z - BrushRange)) &&
            (location.z <= (BrushLocation.z + BrushRange)))
        {
            return BrushColor;
        }
    }

    if (BrushType == 2) //영역 일때
    {
        if (location.x >= StartPos.x && location.x < EndPos.x
            && location.z >= StartPos.z && location.z < EndPos.z)
        {
            return BrushColor;
        }         
    }

    return float3(0, 0, 0);
}

float4 SetSplatColor(float2 uv, float4 diffuse, float4 splatColor, float4 diffuseVal, int splatIndex)
{
    float4 color1 = _splatingMap[0].Sample(_diffuseSampler, uv) * diffuse;
    float4 color2 = _splatingMap[1].Sample(_diffuseSampler, uv) * diffuse;
    float4 color3 = _splatingMap[2].Sample(_diffuseSampler, uv) * diffuse;
    float4 color4 = _splatingMap[3].Sample(_diffuseSampler, uv) * diffuse;

    float4 result;
    if (splatIndex == 0)
        result = (1 - splatColor.r) * diffuseVal + splatColor.r * color1;
    if (splatIndex == 1)
        result = (1 - splatColor.g) * diffuseVal + splatColor.g * color2;
    if (splatIndex == 2)
        result = (1 - splatColor.b) * diffuseVal + splatColor.b * color3;
    if (splatIndex == 3)
        result = (1 - splatColor.a) * diffuseVal + splatColor.a * color4;

    return result;
}

struct VertexInput
{
    float4 position : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float4 splatColor : COLOR1;
    int selectedSplat : INDEX0;
    int selectedAStar : INDEX1;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 oPosition : POSITION0;
    float3 brushColor : COLOR0;
    float4 splatColor : COLOR1;
    int selectedSplat : INDEX0;
    int selectedAStar : INDEX1;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;

    output.position = mul(input.position, _world); //쉐이더로 넘길땐 열우선으로 넘겨야하고 계산은 행우선으로 계산
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);
    output.normal = GetWorldNormal(input.normal, _world);

    output.uv = input.uv;
    output.oPosition = input.position;
    output.brushColor = GetBrushColor(input.position.xyz);
    output.splatColor = input.splatColor;
    output.selectedSplat = input.selectedSplat;
    output.selectedAStar = input.selectedAStar;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuseMap = _diffuseMap.Sample(_diffuseSampler, input.uv);
    float4 diffuse = GetDiffuseColor(_diffuse, _direction, input.normal);

    //grid
    float2 grid = input.oPosition.xz / GridSpacing;
    
    float2 range = abs(frac(grid - 0.5f) - 0.5f);
    float2 speed = fwidth(grid);

    float2 pixelRange = range / speed;
    float lineWeight = saturate(min(pixelRange.x, pixelRange.y) - GridThickness);

    //brush
    float4 brushColor = float4(input.brushColor, 0);

    //diffuse
    float4 diffuseVal = diffuseMap * diffuse;

    //splating
    float4 rColor = SetSplatColor(input.uv, diffuse, input.splatColor, diffuseVal,input.selectedSplat);

    //AStar Color
    float4 closedColor = float4(1, 0, 0, 1);
    float4 openedColor = float4(0, 1, 0, 1);
    float4 pathColor = float4(0, 0, 1, 1);
    float4 obstacleColor = float4(0, 0, 0, 1);
    rColor = float4(1, 1, 1, 1);
    if (input.selectedAStar == 1)
        rColor = closedColor;
    if (input.selectedAStar == 2)
        rColor = openedColor;
    if (input.selectedAStar == 3)
        rColor = pathColor;
    if (input.selectedAStar == 4)
        rColor = obstacleColor;
    //final
    float4 selectedGridColor = float4(0, 1, 0, 1);
    float4 finalColor = float4(1, 1, 1, 1);

    if(GridView == 0)
        lineWeight = 1;

    finalColor = lerp(GridColor, rColor, lineWeight) + brushColor;

    return finalColor;
}