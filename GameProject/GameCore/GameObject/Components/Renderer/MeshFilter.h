#pragma once
#include "./GameCore/GameObject/Components/ComponentBase.h"

class MeshFilter : public ComponentBase
{
public:
	MeshFilter() : ComponentBase() 
	{
	}
	MeshFilter(Mesh* mesh) : ComponentBase() 
	{ 
		this->mesh = mesh; 
	}
	MeshFilter(string meshName) : ComponentBase() 
	{ 
		SetMesh(meshName); 
	}

	virtual ~MeshFilter() {}

	void SetMesh(string name);
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh() const { return mesh; }

private:
	Mesh * mesh;
};