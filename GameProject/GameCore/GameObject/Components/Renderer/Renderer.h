#pragma once
#include "../ComponentBase.h"

/*
렌더링 순서

Update 중 각종 스크립트에서 렌더러에 접근해서,
렌더러의 버퍼 정보를 업데이트한다.

Renderer에서 버퍼를 쉐이더에 입력한 후 shader 실행.
MeshFilter로 그린다.

Skinned Mesh
즉, Bone Animation이 있는 Mesh일 경우
Skinned Mesh Renderer를 사용한다. 
초기화 시 Skeleton을 입력 받고 Skeleton 버퍼를 사용한다.

*/

class Material;
class Renderer : public ComponentBase
{
public:
	Renderer() : ComponentBase()
	{
		dc = D3D::GetDC();
		shader = nullptr;
		material = nullptr;
		meshFilter = nullptr;
		worldBuffer = new WorldBuffer;
	}
	virtual ~Renderer() 
	{
		SAFE_DELETE(worldBuffer);
	}

	void Init(string shaderName, string materialName);

	virtual void Awake();
	virtual void Start() {}
	virtual void Update();
	virtual void Render();
	virtual void PostRender();

	Material* GetMaterial() { return material; }
	void SetShader(string shaderName);

protected:
	Shader * shader;
	Material* material;
	WorldBuffer* worldBuffer;
	class MeshFilter* meshFilter;

	ID3D11DeviceContext* dc;
};


