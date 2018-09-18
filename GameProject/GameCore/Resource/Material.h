#pragma once
#include "Model\ModelMaterial.h"


class MaterialBuffer : public ShaderBuffer
{
public:
	MaterialBuffer()
		: ShaderBuffer(&data, sizeof(Data))
	{
		data.ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		data.emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		data.diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		data.specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		data.shininess = 0;
	}

	void SetAmbient(D3DXCOLOR color) { data.ambient = color; }
	void SetEmissive(D3DXCOLOR color) { data.emissive = color; }
	void SetDiffuse(D3DXCOLOR color) { data.diffuse = color; }
	void SetSpecular(D3DXCOLOR color) { data.specular = color; }
	void SetShininess(float set) { data.shininess = set; }

	D3DXCOLOR GetAmbient() { return data.ambient; }
	D3DXCOLOR GetEmissive() { return data.emissive; }
	D3DXCOLOR GetDiffuse() { return data.diffuse; }
	D3DXCOLOR GetSpecular() { return data.specular; }
	float GetShininess() { return data.shininess; }

	struct Data
	{
		D3DXCOLOR ambient;
		D3DXCOLOR emissive;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		float shininess;
		D3DXVECTOR3 padding;
	};

private:
	Data data;
};

class Material
{
public:
	enum MaterialType { NONE, AMBIENT, DIFFUSE, SPECULAR, EMISSIVE, END };
	Material();
	~Material();

	ID3D11ShaderResourceView* GetAmbientView() { return ambientView; }
	ID3D11ShaderResourceView* GetEmissiveView() { return emissiveView; }
	ID3D11ShaderResourceView* GetDiffuseView() { return diffuseView; }
	ID3D11ShaderResourceView* GetSpecularView() { return specularView; }

	void SetAmbientTexture(wstring file);
	void SetDiffuseTexture(wstring file);
	void SetSpecularTexture(wstring file);
	void SetEmissiveTexture(wstring file);

	void SetAmbient(D3DXCOLOR& color);
	void SetDiffuse(D3DXCOLOR& color);
	void SetSpecular(D3DXCOLOR& color);
	void SetEmissive(D3DXCOLOR& color);
	void SetShininess(float set) { shininess = set; }
	D3DXCOLOR GetAmbient() { return ambient; }
	D3DXCOLOR GetDiffuse() { return diffuse; }
	D3DXCOLOR GetSpecular() { return specular; }
	D3DXCOLOR GetEmissive() { return emissive; }
	float GetShininess() { return shininess; }

	void SetPsBuffer(int slot);

private:
	friend Material * ModelMaterial::MoveMaterial();
	void CreateView(MaterialType type, wstring path, ID3D11ShaderResourceView** view);

	D3DXCOLOR ambient;
	D3DXCOLOR emissive;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	float shininess;

	ID3D11ShaderResourceView* ambientView;
	ID3D11ShaderResourceView* diffuseView;
	ID3D11ShaderResourceView* specularView;
	ID3D11ShaderResourceView* emissiveView;

	MaterialBuffer* materialBuffer;
};