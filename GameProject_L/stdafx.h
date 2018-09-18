#pragma once

#include <Windows.h>
#include <assert.h>
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <functional>
#include <typeinfo>
using namespace std;

#include <atlbase.h>
#include <atlconv.h>

#include <dxgi.h>
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#define FBXSDK_SHARED
#include <fbxsdk.h>
#pragma comment(lib, "libfbxsdk.lib")
using namespace fbxsdk;

#include <imgui.h>
#include <imguiDx11.h>
#pragma comment(lib, "imgui.lib")

#define SAFE_RELEASE(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SAFE_DELETE(p){ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARRAY(p){ if(p){ delete [] (p); (p) = NULL; } }

const wstring Contents = L"../_Contents/";
const wstring Shaders = L"../_Shaders/";
const wstring Textures = L"../_Contents/Textures/";

#include "./Systems/D3D.h"
#include "./Systems/Keyboard.h"
#include "./Systems/Mouse.h"
#include "./Systems/Time.h"

#include "./Renderers/GUISettings.h"
#include "./Renderers/Renderers.h"

#include "./Object/Shader/ShaderBuffer.h"
#include "./Object/Shader/Shader.h"

#include "./Systems/Renders/VertexLayouts.h"
#include "./Systems/Renders/States.h"
#include "./Systems/Renders/BackBuffer.h"
#include "./Systems/Renders/GlobalBuffers.h"
#include "./Systems/Renders/RenderOptions.h"
#include "./Systems/Renders/RenderTarget.h"
#include "./Systems/Camera/Camera.h"

#include "./Utilities/Warehouse.h"
#include "./Utilities/BinaryFile.h"
#include "./Utilities/String.h"
#include "./Utilities/Texture.h"
#include "./Utilities/ImageMaker.h"
#include "./Utilities/PerlinNoise.h"
#include "./Utilities/Perlin.h"

#include "./Object/Components/IComponent.h"
#include "./Object/Mesh/IMeshData.h"
#include "./Object/IGameObject.h"