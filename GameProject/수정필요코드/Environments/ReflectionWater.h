#pragma once

class VSReflectionWaterBuffer : public ShaderBuffer
{
public:
	VSReflectionWaterBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.normalMapTiling = D3DXVECTOR2(0.01f, 0.02f);
	}

	struct Struct
	{
		D3DXVECTOR2 normalMapTiling;
		D3DXVECTOR2 paddinf;
	};

	Struct Data;
};

class PSReflectionWaterBuffer : public ShaderBuffer
{
public:
	PSReflectionWaterBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.refractionTint = D3DXVECTOR4(0.0f, 0.8f, 1.0f, 1.0f);
		Data.waterTranslation = 0.0f;
		Data.reflectRefractScale = 0.03f;
		Data.specularShininess = 200.0f;
	}

	struct Struct
	{
		D3DXVECTOR4 refractionTint;
		float waterTranslation;
		float reflectRefractScale;
		float specularShininess;
		float padding;
	};

	Struct Data;
};

class ReflectionWater
{
public:
	ReflectionWater();
	~ReflectionWater();

	void Update(struct ExecuteValues* values);
	void Render(ID3D11ShaderResourceView *refractionTexture, ID3D11ShaderResourceView *reflectionTexture);
	float GetWaterHeight()
	{
		return waterHeight;
	}

private:
	void CreateData(float WaterRadius = 128.0f);
	void CreateBuffers();
	void CreateTextures();

private:
	typedef VertexTexture VertexType;

	Shader* shader;
	VSReflectionWaterBuffer* vsBuffer;
	PSReflectionWaterBuffer* psBuffer;

	WorldBuffer* worldBuffer;

	vector<VertexType> vertex;
	vector<UINT> index;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	UINT vertexCount;
	UINT indexCount;

	ID3D11ShaderResourceView *normalTexture;

	D3DXMATRIX world;

	float waterTranslation;
	float waterHeight;
};