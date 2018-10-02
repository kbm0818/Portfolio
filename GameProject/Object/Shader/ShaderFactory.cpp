#include "stdafx.h"
#include "ShaderFactory.h"

ShaderFactory::ShaderFactory()
{

}

ShaderFactory::~ShaderFactory()
{
	for (auto it = datas.begin(); it != datas.end(); it++)
	{
		SAFE_DELETE(it->second);
	}
}


Shader* ShaderFactory::Create(wstring keyName, ShaderType type)
{
	map<wstring, Shader*>::iterator it;
	Shader* temp = nullptr;

	it = datas.find(keyName);

	if (it == datas.end())
	{
		temp = new Shader(keyName, type);
		datas.insert(make_pair(keyName, temp));
	}
	else
	{
		temp = it->second;
	}

	return temp;
}
