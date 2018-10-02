#pragma once

//#include "../Systems/Camera/Camera.h"
//#include "../Systems/Camera/Perspective.h"
//#include "../Systems/Camera/Viewport.h"
//#include "../Systems/GameSystems/Frustum.h"

struct GlobalValues
{
	struct GuiSettings* GuiSettings;

	class GlobalLightBuffer* GlobalLight;
	class ViewProjectionBuffer* GlobalViewProjection;
	class GlobalCameraBuffer* GlobalCamera;

	class Perspective* Perspective;
	class Viewport* Viewport;
	class Camera* MainCamera;

	class Frustum* Frustum;
	class TerrainHeightData* TerrainHeightData;

	class GameObjectFactory* GameObjectFactory;
	class ShaderFactory* ShaderFactory;
	class SRVFactory* SRVFactory;

	class RenderOptions* RenderOption;
};

class Renderers
{
public:
	Renderers(GlobalValues* values)
	{
		this->values = values;
	}

	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual void ResizeScreen() = 0;

protected:
	GlobalValues* values;
};