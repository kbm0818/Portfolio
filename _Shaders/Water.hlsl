#include "Constants.hlsl"

Texture2D _map[3] : register(t0);
// 0 -> refractionTexture
// 1 -> reflectionTexture
// 2 -> normalTexture
SamplerState _samp;

cbuffer VS_Reflection :register(b2)
{
	float2 normalMapTiling;
	float2 vsReflectionPadding;
};

struct VertexInput
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
};

cbuffer PS_Reflection :register(b1)
{
	float4 refractionTint;
	float waterTranslation;
	float reflectRefractScale;
	float specularShininess;
	float psReflectionPadding;
};

struct PixelInput
{
	float4 position : SV_POSITION;
	float2 uv1 : TEXCOORD0;
	float2 uv2 : TEXCOORD1;
	float4 reflectionPosition : TEXCOORD2;
	float4 refractionPosition : TEXCOORD3;
	float3 viewDirection : TEXCOORD4;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

    output.position = mul(float4(input.position, 1.0f), _world);
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _projection);

	matrix reflectProjectWorld = mul(_reflection, _projection);
	reflectProjectWorld = mul(_world, reflectProjectWorld);

	output.reflectionPosition = mul(float4(input.position, 1.0f), reflectProjectWorld);

	matrix viewProjectWorld = mul(_view, _projection);
	viewProjectWorld = mul(_world, viewProjectWorld);

	output.refractionPosition = mul(float4(input.position, 1.0f), viewProjectWorld);

	float4 worldPosition = mul(float4(input.position,1.0f), _world);
	
	output.viewDirection = _viewPosition.xyz - worldPosition.xyz;

	output.uv1 = input.uv / normalMapTiling.x;
	output.uv2 = input.uv / normalMapTiling.y;

	return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
	float4 normalMap1;
	float4 normalMap2;
	float3 normal1;
	float3 normal2;
	float3 normal;
	float2 refractTexCoord;
	float2 reflectTexCoord;
	float4 reflectionColor;
	float4 refractionColor;
	float3 heightView;
	float r;
	float fresnelFactor;
	float4 color;
	float3 reflection;
	float specular;


	// Move the position the water normal is sampled from to simulate moving water.	
	input.uv1.y += waterTranslation;
	input.uv2.y += waterTranslation;

	// Sample the normal from the normal map texture using the two different tiled and translated coordinates.
	normalMap1 = _map[2].Sample(_samp, input.uv1);
	normalMap2 = _map[2].Sample(_samp, input.uv2);

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal1 = (normalMap1.rgb * 2.0f) - 1.0f;
	normal2 = (normalMap2.rgb * 2.0f) - 1.0f;

	// Combine the normals to add the normal maps together.
	normal = normalize(normal1 + normal2);

	// Calculate the projected refraction texture coordinates.
	refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
	refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	// Re-position the texture coordinate sampling position by the scaled normal map value to simulate the rippling wave effect.
	reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
	refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);

	// Sample the texture pixels from the textures using the updated texture coordinates.
	reflectionColor = _map[1].Sample(_samp, reflectTexCoord);
	refractionColor = _map[0].Sample(_samp, refractTexCoord);

	// Combine the tint with the refraction color.
	refractionColor = saturate(refractionColor * refractionTint);

	// Get a modified viewing direction of the camera that only takes into account height.
	heightView.x = input.viewDirection.y;
	heightView.y = input.viewDirection.y;
	heightView.z = input.viewDirection.y;

	// Now calculate the fresnel term based solely on height.
	r = (1.2f - 1.0f) / (1.2f + 1.0f);
	fresnelFactor = max(0.0f, min(1.0f, r + (1.0f - r) * pow(1.0f - dot(normal, heightView), 2)));

	// Combine the reflection and refraction results for the final color using the fresnel factor.
	color = lerp(reflectionColor, refractionColor, fresnelFactor);

	// Calculate the reflection vector using the normal and the direction of the light.
	reflection = -reflect(normalize(_direction), normal);

	// Calculate the specular light based on the reflection and the camera position.
	specular = dot(normalize(reflection), normalize(input.viewDirection));

	// Check to make sure the specular was positive so we aren't adding black spots to the water.
	if (specular > 0.0f)
	{
		// Increase the specular light by the shininess value.
		specular = pow(specular, specularShininess);

		// Add the specular to the final color.
		color = saturate(color + specular);
	}

	return color;
}