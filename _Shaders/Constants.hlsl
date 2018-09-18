cbuffer VS_ViewProjection : register(b0)
{
	matrix _view;
    matrix _projection;
	matrix _reflection;
}

cbuffer VS_World : register(b1)
{
	matrix _world;
}

cbuffer VS_Camera : register(b2)
{
	float3 _cameraPosition;
	float vpPadding;
}

cbuffer PS_Sun : register(b0)
{
    float3 _direction;
    float _sunPadding;
}