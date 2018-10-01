cbuffer VS_ViewProjection : register(b0)
{
	matrix _view;
    matrix _projection;
	matrix _orthoView;
	matrix _orthoProjection;
	matrix _reflection;
}

cbuffer VS_World : register(b1)
{
	matrix _world;
}

cbuffer VS_Camera : register(b2)
{
	float3 _cameraPosition;
	float3 _playerPosition;
	float2 cameraPadding;
}

cbuffer VS_Fog : register(b9)
{
	float4 _fogColor;
	float _zFogStart;
	float _zFogEnd;
	float _yFogStart;
	float _yFogEnd;
}

cbuffer VS_LightMatrix : register(b10)
{
	matrix _lightView;
	matrix _lightProjection;
	float3 _lightPosition;
	float _lightPadding;
}

cbuffer PS_LightDirection : register(b10)
{
	float4 _ambientColor;
	float4 _diffuseColor;
    float3 _direction;
    float _sunPadding;
}