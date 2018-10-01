#include "000_Header.hlsl"

cbuffer PS_Sky : register(b2)
{
    float4 HorizonColorTwilight;
    float4 HorizonColorDay;
    float4 HorizonColorNight;

    float4 CeilingColorTwilight;
    float4 CeilingColorDay;
    float4 CeilingColorNight;

    float Height;
    float Time;
}

struct PixelInput
{
    float4 position : SV_POSITION;
    float4 oPosition : OUTPUT0;
    float2 uv : UV0;
};

PixelInput VS(VertexTexture input)
{
    PixelInput output;

    output.position = mul(input.position, _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

    output.oPosition = input.position;
    output.uv = input.uv;

    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 horizon;
    float4 ceiling;
    
    if(_direction.y >0)
    {
        float amount = min(_direction.y * 1.5f, 1);
        horizon = lerp(HorizonColorTwilight, HorizonColorNight, amount);
        ceiling = lerp(CeilingColorTwilight, CeilingColorNight, amount);
    }
    else
    {
        float amount = min(_direction.y * -1.5f, 1);
        horizon = lerp(HorizonColorTwilight, HorizonColorDay, amount);
        ceiling = lerp(CeilingColorTwilight, CeilingColorDay, amount);
    }

    float3 color = lerp(horizon, ceiling, saturate(input.oPosition.y / Height));

    float3 temp = float3(1.2f, 0.8f, 0.0f);
    float dist = saturate(1 - distance(input.oPosition.xyz, _direction)) / 2;
    float intensity = (1 - _sunIntensity) / _overcast;

    color += temp * dist * intensity;

    float horizonLerp = saturate(lerp(0, 1, input.oPosition.y * 1.5f));
    float directionLerp = lerp(0, 1, max(_direction.y, 0));

    float starAlpha = saturate(horizonLerp * directionLerp);
    
    float2 starUv = input.uv * 16;
    starUv.xy += Time / 50;

    color += _diffuseMap.Sample(_diffuseSampler, starUv) * starAlpha * 0.6f / (_overcast * 2);

    return float4(color, 1);
}