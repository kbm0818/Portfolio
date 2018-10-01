cbuffer VS_ViewProjection : register(b0)
{
    matrix _view;
    matrix _projection;
}

cbuffer VS_World : register(b1) //WorldBuffer 가 담당
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
    float4 color : COLOR0;
    float4 color2 : COLOR1;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float4 color2 : COLOR1;
    float3 normal : OUTPUT0;
    float3 brushColor : COLOR2;
};

SamplerState _sampler : register(s0);
Texture2D _map[5] : register(t0); //항상 번호 일치하게

float3 GetBrushColor(float3 location)
{
    if(_brushType == 0)
        return float3(0, 0, 0);

    if(_brushType == 1) //사각형 일때
    {
        if ((location.x >= (_brushLocation.x - _brushRange))&&
            (location.x <= (_brushLocation.x + _brushRange))&&
            (location.z >= (_brushLocation.z - _brushRange))&&
            (location.z <= (_brushLocation.z + _brushRange)))
        {
            return _brushColor;
        }
    }

    if(_brushType == 2)//원
    {
        float dx = location.x - _brushLocation.x;
        float dy = location.z - _brushLocation.z;

        float dist = sqrt(dx * dx + dy * dy);
        if(dist <= _brushRange)
            return _brushColor;
    }

    return float3(0, 0, 0);
}

PixelInput VS(VertexInput input) //IA 에서 일로넘어옴 (IA -> VS) VS는 정점만 처리
{
    PixelInput output;
    output.brushColor = GetBrushColor(input.position.xyz); //그냥 로컬좌표로만 넘겼음
    output.position = mul(input.position, _world); //월드로 바꿈
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.color = input.color;
    output.color2 = input.color2;
    output.normal = mul(input.normal, (float3x3) _world);
    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float3 light = _direction * -1;
    float intensity = saturate(dot(input.normal, light)); //saturate 0~1

    float4 brushColor = float4(input.brushColor, 0);

    float4 diffuseMap = _map[0].Sample(_sampler, input.uv) * intensity;
    float4 alphaMap = input.color2;
    float4 color1 = _map[1].Sample(_sampler, input.uv) * intensity;
    float4 color2 = _map[2].Sample(_sampler, input.uv) * intensity;
    float4 color3 = _map[3].Sample(_sampler, input.uv) * intensity;
    float4 color4 = _map[4].Sample(_sampler, input.uv) * intensity;


    float4 result = 
    (1 - alphaMap.a) * diffuseMap + alphaMap.a * color1 * 4
    + (1 - alphaMap.r) * diffuseMap + alphaMap.r * color2 * 4
    + (1 - alphaMap.g) * diffuseMap + alphaMap.g * color3 * 4
    + (1 - alphaMap.b) * diffuseMap + alphaMap.b * color4 * 4;

    result = result / 4;
    return result + brushColor;
}