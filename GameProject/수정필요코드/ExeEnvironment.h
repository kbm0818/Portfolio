#pragma once
#include "Execute.h"

class ExeEnvironment : public Execute
{
public:
	ExeEnvironment(ExecuteValues* values);
	~ExeEnvironment();

	void Update();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	class RenderTarget* refractTarget;
	class RenderTarget* reflectTarget;

	class Shader* reflectShader;
	class ReflectShaderBuffer* reflectShaderBuffer;

	class Sky* sky;
	class SkyPlane_Perlin* skyPlane;
	class LensFlare* lensFlare;
	class ReflectionWater* water;
	class Billboard* tree;

	bool bDrawSky;
	bool inWater;

	D3DXVECTOR4 reflectClip;
	D3DXVECTOR4 refractClip;
};