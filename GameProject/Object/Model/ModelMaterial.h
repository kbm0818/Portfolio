#pragma once

enum ViewList
{
	AMBIENT = 0,
	DIFFUSE,
	EMISSIVE,
	NORMAL,
	SPECULAR,
};

class Shader;
class MaterialBuffer;
class ModelMaterial
{
public:
	ModelMaterial();
	~ModelMaterial();

	bool CanRender() { return shader != NULL; }

	UINT GetNumber() { return number; }
	string GetName() { return name; }

	MaterialBuffer* GetModelBuffer() { return shaderBuffer; }

	void SetPSBuffer(UINT slot);

	void Load(BinaryReader* r, GlobalValues* values);

private:
	UINT number;
	string name;

	Shader* shader;
	MaterialBuffer* shaderBuffer;

	vector<ID3D11ShaderResourceView*> views; 
	//0: ambient, 1: diffuse, 2:specular, 3.emissive, 4.normal

	GlobalValues* values;
};