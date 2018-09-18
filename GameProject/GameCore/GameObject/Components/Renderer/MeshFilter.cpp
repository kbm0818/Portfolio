#include "stdafx.h"
#include "MeshFilter.h"

void MeshFilter::SetMesh(string name)
{
	SetMesh(AssetManager::Get()->GetMesh(name));
}

void MeshFilter::SetMesh(Mesh * mesh)
{
	this->mesh = mesh;
}
