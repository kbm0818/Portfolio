#pragma once

enum class HeightMapFileType
{
	Texture = 0,
	binary,
};
class TerrainHeightData
{
public:
	TerrainHeightData();
	~TerrainHeightData();

	void Init(wstring path, HeightMapFileType type, float scale = 1.0f, float divideValue = 15.0f);
	float GetY(D3DXVECTOR3& position);
	void SaveBinary(wstring path);

private:
	void LoadBinary(wstring path);
	
	void LoadHeightMap(wstring path, float scale, float divideValue);
	void CalcNormal();
	void CalcUv();

private:
	friend class TerrainMeshFactory;
	friend class TerrainQuadTree; // Å×½ºÆ®

	typedef VertexTextureNormal VertexType;

	UINT width, height;
	float scale;

	vector<VertexType> heightData;
};