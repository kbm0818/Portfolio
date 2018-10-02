#include "stdafx.h"
#include "MeshFactory.h"

MeshFactory::MeshFactory()
{
}

MeshFactory::~MeshFactory()
{
	for (auto it = datas.begin(); it != datas.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
}