#pragma once 

class ShaderFactory
{
public:
	ShaderFactory();
	~ShaderFactory();

	Shader* Create(wstring keyName, ShaderType type = ShaderType::VP);

private:
	map<wstring, Shader*> datas;
};