#include "Constants.hlsl"

Texture2D _cloudTexture1 : register(t0);
Texture2D _cloudTexture2 : register(t1);
SamplerState _samp;

cbuffer SkyPlaneBuffer : register(b1) // PS(1)
{
	float firstTranslationX;
	float firstTranslationZ;
	float secondTranslationX;
	float secondTranslationZ;
	float brightness;
	float3 padding;
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

PixelInput VS(VertexInput input)
{
	PixelInput output;

	input.position.w = 1.0f;

	output.position = mul(input.position, _world);
	output.position = mul(output.position, _view);
	output.position = mul(output.position, _projection);

	output.uv = input.uv;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float2 sampleLocation;
	float4 textureColor1;
	float4 textureColor2;
	float4 finalColor;

	// Translate the position where we sample the pixel from using the first texture translation values.
	sampleLocation.x = input.uv.x + firstTranslationX;
	sampleLocation.y = input.uv.y + firstTranslationZ;

	// Sample the pixel color from the first cloud texture using the sampler at this texture coordinate location.
	textureColor1 = _cloudTexture1.Sample(_samp, sampleLocation);

	// Translate the position where we sample the pixel from using the second texture translation values.
	sampleLocation.x = input.uv.x + secondTranslationX;
	sampleLocation.y = input.uv.y + secondTranslationZ;

	// Sample the pixel color from the second cloud texture using the sampler at this texture coordinate location.
	textureColor2 = _cloudTexture2.Sample(_samp, sampleLocation);

	// Combine the two cloud textures evenly.
	finalColor = lerp(textureColor1, textureColor2, 0.5f);

	// Reduce brightness of the combined cloud textures by the input brightness value.
	finalColor = finalColor * brightness;

	return finalColor;
}