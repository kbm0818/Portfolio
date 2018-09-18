#pragma once

class ModelBuffer : public ShaderBuffer
{
public:
	ModelBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		data.ambient = D3DXCOLOR(0, 0, 0, 1);
		data.diffuse = D3DXCOLOR(0, 0, 0, 1);
		data.specular = D3DXCOLOR(0, 0, 0, 1);
		data.emissive = D3DXCOLOR(0, 0, 0, 1);
		data.normal = D3DXCOLOR(0, 0, 0, 1);

		//D3DXMatrixIdentity(&data.boneScale);
		D3DXMatrixScaling(&data.boneScale, 0.1f, 0.1f, 0.1f);

		for (int i = 0; i < 100; i++)
			D3DXMatrixIdentity(&data.boneArray[i]);

		data.isSkinning = false;
	}

	void SetSkinning(bool isSkinning)
	{
		data.isSkinning = (UINT)isSkinning;
	}

	void SetBoneArray(D3DXMATRIX* matrix, UINT count)
	{
		memcpy(data.boneArray, matrix, count * sizeof(D3DXMATRIX));

		for (UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.boneArray[i], &data.boneArray[i]);
	}

	void SetBoneScale(D3DXMATRIX matrix)
	{
		data.boneScale = matrix;
	}

	D3DXCOLOR GetAmbient() { return data.ambient; }
	D3DXCOLOR GetDiffuse() { return data.diffuse; }
	D3DXCOLOR GetSpecular() { return data.specular; }
	D3DXCOLOR GetEmissive() { return data.emissive; }
	D3DXCOLOR GetNormal() { return data.normal; }

	void SetAmbient(D3DXCOLOR& color) { data.ambient = color; }
	void SetDiffuse(D3DXCOLOR& color) { data.diffuse = color; }
	void SetSpecular(D3DXCOLOR& color) { data.specular = color; }
	void SetEmissive(D3DXCOLOR& color) { data.emissive = color; }
	void SetNormal(D3DXCOLOR& color) { data.normal = color; }

	D3DXMATRIX GetBoneScale() { return data.boneScale; }

	struct Data
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		D3DXCOLOR emissive;
		D3DXCOLOR normal;

		D3DXMATRIX boneScale;
		D3DXMATRIX boneArray[100];
		UINT isSkinning;
		D3DXVECTOR3 padding;
	};

private:
	Data data;
};

class MaterialBuffer : public ShaderBuffer
{
public:
	MaterialBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		data.ambient = D3DXCOLOR(0, 0, 0, 1);
		data.diffuse = D3DXCOLOR(0, 0, 0, 1);
		data.specular = D3DXCOLOR(0, 0, 0, 1);
		data.emissive = D3DXCOLOR(0, 0, 0, 1);
		data.normal = D3DXCOLOR(0, 0, 0, 1);
		data.shininess = 0.0f;
	}

	D3DXCOLOR GetAmbient() { return data.ambient; }
	D3DXCOLOR GetDiffuse() { return data.diffuse; }
	D3DXCOLOR GetSpecular() { return data.specular; }
	D3DXCOLOR GetEmissive() { return data.emissive; }
	D3DXCOLOR GetNormal() { return data.normal; }

	void SetAmbient(D3DXCOLOR& color) { data.ambient = color; }
	void SetDiffuse(D3DXCOLOR& color) { data.diffuse = color; }
	void SetSpecular(D3DXCOLOR& color) { data.specular = color; }
	void SetEmissive(D3DXCOLOR& color) { data.emissive = color; }
	void SetNormal(D3DXCOLOR& color) { data.normal = color; }
	void SetShininess(float sh) { data.shininess = sh; }

	struct Data
	{
		D3DXCOLOR ambient;
		D3DXCOLOR diffuse;
		D3DXCOLOR specular;
		D3DXCOLOR emissive;
		D3DXCOLOR normal;
		float shininess;
		D3DXVECTOR3 padding;
	};

private:
	Data data;
};

class SkeletonBuffer : public ShaderBuffer
{
public:
	SkeletonBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		for (int i = 0; i < 100; i++)
			D3DXMatrixIdentity(&data.boneArray[i]);

		data.isSkinning = false;
	}

	void SetSkinning(bool isSkinning)
	{
		data.isSkinning = (UINT)isSkinning;
	}

	void SetBoneArray(D3DXMATRIX* matrix, UINT count)
	{
		memcpy(data.boneArray, matrix, count * sizeof(D3DXMATRIX));

		for (UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.boneArray[i], &data.boneArray[i]);
	}

	struct Data
	{
		D3DXMATRIX boneArray[100];
		UINT isSkinning;
		D3DXVECTOR3 padding;
	};

private:
	Data data;
};
