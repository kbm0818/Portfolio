#pragma once
#include "../ComponentBase.h"

/*
������ ����

Update �� ���� ��ũ��Ʈ���� �������� �����ؼ�,
�������� ���� ������ ������Ʈ�Ѵ�.

Renderer���� ���۸� ���̴��� �Է��� �� shader ����.
MeshFilter�� �׸���.

Skinned Mesh
��, Bone Animation�� �ִ� Mesh�� ���
Skinned Mesh Renderer�� ����Ѵ�. 
�ʱ�ȭ �� Skeleton�� �Է� �ް� Skeleton ���۸� ����Ѵ�.

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


