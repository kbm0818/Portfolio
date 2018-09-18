#pragma once

class ModelBuffer;
class ModelMaterial
{
public:
	bool CanRender() { return shader != NULL; }

	UINT GetNumber() { return number; }
	string GetName() { return name; }

	void SetShader(string file);

	void SetAmbient(D3DXCOLOR& color);
	void SetDiffuse(D3DXCOLOR& color);
	void SetSpecular(D3DXCOLOR& color);
	void SetEmissive(D3DXCOLOR& color);
	void SetNormal(D3DXCOLOR& color);

	string GetAmbientFile() { return ambientFile; }
	string GetDiffuseFile() { return diffuseFile; }
	string GetSpecularFile() { return specularFile; }
	string GetEmissiveFile() { return emissiveFile; }
	string GetNormalFile() { return normalFile; }


	void SetAmbientTexture(string file);
	void SetDiffuseTexture(string file);
	void SetSpecularTexture(string file);
	void SetEmissiveTexture(string file);
	void SetNormalTexture(string file);

	void SetPSBuffer();

private:
	friend class MoModel;
	friend class MoMaterial;
	friend class Model;

	ModelMaterial();
	~ModelMaterial();

	void CreateTexture(string file, ID3D11ShaderResourceView** view);

	typedef VertexTextureNormalTangentBinormalBlend VertexType;
	
	UINT number;
	string name;

	Shader* shader;
	ModelBuffer* shaderBuffer;
	string shaderFile;

	string ambientFile;
	string diffuseFile;
	string specularFile;
	string emissiveFile;
	string normalFile;

	ID3D11ShaderResourceView* ambientView;
	ID3D11ShaderResourceView* diffuseView;
	ID3D11ShaderResourceView* specularView;
	ID3D11ShaderResourceView* emissiveView;
	ID3D11ShaderResourceView* normalView;
};