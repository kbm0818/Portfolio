#include "stdafx.h"
#include "Model.h"
#include "ModelMesh.h"

Model::Model(wstring name)
	: name(name)
{
}

Model::~Model()
{
	for (size_t i = 0; i < parts.size(); i++)
		SAFE_DELETE(parts[i]);
}

