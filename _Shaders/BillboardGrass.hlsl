#include "000_Header.hlsl"

cbuffer VS_Wind : register(b2)
{
    float3 WindDirection;
    float WindWaveSize;
	
    float WindRandomness;
    float WindSpeed;
    float WindAmount;
    float WindTime;
	
    float BillboardWidth;
    float BillboardHeight;
    float Random;
    float Padding;
}


struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
};

float4 PS(PixelInput input) : SV_TARGET
{
    float4 diffuse = _diffuseMap.Sample(_diffuseSampler, input.uv);

    //if(diffuse.a < 0.5f)
    //    discard; //discard 는 바로 리턴됨
    
    clip(diffuse.a - 0.5f); //clip은 리턴은안됨


    return diffuse;
}

PixelInput VS(VertexTextureNormal input)
{
    PixelInput output;

    float squishFactor = 0.75 + abs(Random) / 2.0f;

    float width = BillboardWidth * squishFactor;
    float height = BillboardHeight / squishFactor;

    if (Random < 0)
        width = -width;

    float3 viewDirection = _view._m02_m12_m22;
    float3 rightVector = normalize(cross(viewDirection, input.normal));
    float3 position = mul(input.position, _world);

    // Offset to the left or right.
    position += rightVector * (input.uv.x - 0.5) * width;
    
    // Offset upward if we are one of the top two vertices.
    position += input.normal * (1 - input.uv.y) * height;

    float waveOffset = dot(position, WindDirection) * WindWaveSize;
    
    waveOffset += Random * WindRandomness;
    
    float wind = sin(WindSpeed*WindSpeed + waveOffset) * WindAmount;
    
    wind *= (1 - input.uv.y);
    
    position += WindDirection * wind;

    // Apply the camera transform.
    output.position = mul(float4(position, 1), _view);
    output.position = mul(output.position, _projection);

    output.uv = input.uv;   
    
    return output;
}