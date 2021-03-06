float4 NormalColor(float4 ambientColor, float4 diffuseColor, float3 lightDirection, float3 normal)
{
	float4 color = ambientColor;
	float3 lightDir = -lightDirection;

	float lightIntensity = saturate(dot(normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		color +=(diffuseColor * lightIntensity);
	}

	color = saturate(color);

	return float4(color.rgb, 1.0f);
}

float4 SpecularNormalColor(float4 ambientColor, float4 diffuseColor, float4 specular, float specularPower, float3 normal,float3 viewDirection, float3 lightDirection)
{
	float4 color = ambientColor;
	float3 lightDir = -lightDirection;

	float lightIntensity = saturate(dot(normal, lightDir));

	if (lightIntensity > 0.0f)
	{
		float3 reflection = normalize(2 * lightIntensity * normal - lightDir);
		float4 specularIntensity = pow(saturate(dot(reflection, viewDirection)), specularPower);
		color += (diffuseColor * lightIntensity) + (specularIntensity * specular);
	}

	color = saturate(color);

	return float4(color.rgb, 1.0f);
}

float2 parallaxMapping(float3 toEyeTangent, float2 uv, float height, float heightScale)
{
	float2 offset = toEyeTangent.xy * height * heightScale;

	return uv + offset;
}

float2 parallaxOcclusionMapping(
	float3 _toEyeTangent, float3 _toEye, float3 _normal,
	float2 _uv, Texture2D _map, SamplerState _samp, 
	float _heightScale, int _maxSampleCount, int _minSampleCount
)
{
	float2 maxParallaxOffset = -_toEyeTangent.xy * _heightScale / _toEyeTangent.z;

	int sampleCount = (int)lerp(_maxSampleCount, _minSampleCount,
		dot(_toEye, _normal));
	float zStep = 1.0f / (float)sampleCount;
	float2 texStep = maxParallaxOffset * zStep;

	float2 dx = ddx(_uv);
	float2 dy = ddy(_uv);

	int sampleIndex = 0;
	float2 currTexOffset = 0;
	float2 prevTexOffset = 0;
	float2 finalTexOffset = 0;
	float currRayZ = 1.0f - zStep;
	float prevRayZ = 1.0f;
	float currHeight = 0.0f;
	float prevHeight = 0.0f;

	// Ray trace the heightfield.
	while (sampleIndex < sampleCount + 1)
	{
		currHeight = _map.SampleGrad(_samp,
			_uv + currTexOffset, dx, dy).x;
		// Did we cross the height profile?
		if (currHeight > currRayZ)
		{
			// Do ray/line segment intersection and compute final tex offset.
			float t = (prevHeight - prevRayZ) /
				(prevHeight - currHeight + currRayZ - prevRayZ);
			finalTexOffset = prevTexOffset + t * texStep;
			// Exit loop.
			sampleIndex = sampleCount + 1;
		}
		else
		{
			++sampleIndex;
			prevTexOffset = currTexOffset;
			prevRayZ = currRayZ;
			prevHeight = currHeight;
			currTexOffset += texStep;
			// Negative because we are going "deeper" into the surface.
			currRayZ -= zStep;
		}
	}

	return _uv + finalTexOffset;
}

float4 GetShadowSpecularColor(float4 _ambientColor, float4 _diffuseColor, float _bias,
	float4 _lightViewPosition, float3 _normal, float3 _viewDirection, float3 _lightDirection,
	Texture2D _map, SamplerState _samp, float4 _specularColor, float _specularPower)
{
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
	float lightIntensity;
	float4 color = _ambientColor;

	projectTexCoord.x = _lightViewPosition.x / _lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -_lightViewPosition.y / _lightViewPosition.w / 2.0f + 0.5f;

	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = _map.Sample(_samp, projectTexCoord).r;
		lightDepthValue = _lightViewPosition.z / _lightViewPosition.w;
		lightDepthValue = lightDepthValue - _bias;

		if (lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(_normal, _viewDirection));

			if (lightIntensity > 0.0f)
			{
				float3 reflection = normalize(2 * lightIntensity * _normal - _lightDirection);
				float4 specularIntensity = pow(saturate(dot(reflection, _viewDirection)), _specularPower);

				color += (_diffuseColor * lightIntensity) + (specularIntensity * _specularColor);
				color = saturate(color);
			}
		}
	}

	return color;
}