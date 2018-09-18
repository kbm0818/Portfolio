#pragma once

class ModelBuffer;
class Material;
class ModelMaterial
{
public:
	enum MaterialType { NONE, AMBIENT, DIFFUSE, SPECULAR, EMISSIVE, END };
	ModelMaterial();
	ModelMaterial(ModelMaterial& copy);

	~ModelMaterial();

	UINT GetNumber() { return number; }

	wstring GetAmbientFile() { return ambientFile; }
	wstring GetDiffuseFile() { return diffuseFile; }
	wstring GetSpecularFile() { return specularFile; }
	wstring GetEmissiveFile() { return emissiveFile; }

	void SetAmbientTexture(wstring file);
	void SetDiffuseTexture(wstring file);
	void SetSpecularTexture(wstring file);
	void SetEmissiveTexture(wstring file);

	ID3D11ShaderResourceView* GetAmbientView() { return ambientView; }
	ID3D11ShaderResourceView* GetEmissiveView() { return emissiveView; }
	ID3D11ShaderResourceView* GetDiffuseView() { return diffuseView; }
	ID3D11ShaderResourceView* GetSpecularView() { return specularView; }

	void SetAmbient(D3DXCOLOR& color);
	void SetDiffuse(D3DXCOLOR& color);
	void SetSpecular(D3DXCOLOR& color);
	void SetEmissive(D3DXCOLOR& color);
	D3DXCOLOR GetAmbient() { return ambient; }
	D3DXCOLOR GetDiffuse() { return diffuse; }
	D3DXCOLOR GetSpecular() { return specular; }
	D3DXCOLOR GetEmissive() { return emissive; }
	float GetShininess() { return shininess; }

	Material* MoveMaterial();

private:
	friend class ModelLoader;
	friend class Model;

	void CreateView(MaterialType type, wstring path, ID3D11ShaderResourceView** view);

	UINT number;
	D3DXCOLOR ambient;
	D3DXCOLOR emissive;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	float shininess;

	wstring ambientFile;
	wstring emissiveFile;
	wstring diffuseFile;
	wstring specularFile;

	ID3D11ShaderResourceView* ambientView;
	ID3D11ShaderResourceView* diffuseView;
	ID3D11ShaderResourceView* specularView;
	ID3D11ShaderResourceView* emissiveView;
};