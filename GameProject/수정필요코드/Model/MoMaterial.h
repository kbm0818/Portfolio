#pragma once

class ModelMaterial;
class BinaryWriter;
class MoMaterial
{
public:
	UINT GetNumber() { return number; }

private:
	friend class MoLoader;
	friend class MoModel;

	MoMaterial(int number, FbxSurfaceMaterial* material);
	~MoMaterial();	

	void Write(BinaryWriter* w);
	static void Read(BinaryReader* r, ModelMaterial* modelMaterial);
	static void Write(BinaryWriter* w, ModelMaterial* modelMaterial);

	D3DXCOLOR GetProperty
	(
		FbxSurfaceMaterial* material
		, const char* name, const char* factorName
		, string* textureName
	);
	D3DXCOLOR GetProperty
	(
		FbxSurfaceMaterial* material
		, const char* name
		, string* textureName
	);

	UINT number;
	string name;
	string shaderFile;

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXCOLOR emissive;
	D3DXCOLOR normal;
	float shininess;

	string ambientFile;
	string diffuseFile;
	string specularFile;
	string emissiveFile;
	string normalFile;
};