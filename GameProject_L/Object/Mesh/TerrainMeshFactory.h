#pragma once

class MeshNodeData;
struct MeshNode;
class TerrainMeshFactory
{
public:
	TerrainMeshFactory();
	~TerrainMeshFactory();

	MeshNodeData* Create(GlobalValues* values);

private:
	void CreateFullData(GlobalValues* exValue);
	void CreateQuadTreeData(GlobalValues* exValue);

	void CalculateMeshDimensions(int vertexCount, float &centerX, float &centerZ, float &meshWidth);
	void CreateTreeNode(MeshNode *node, float positionX, float positionZ, float width);
	int CountTriangles(float positionX, float positionZ, float width);
	bool IsTriangleContained(int index, float positionX, float positionZ, float width);

private:
	typedef VertexTextureNormalTangentBinormal VertexType;
	vector<VertexType> fullVertexData;
	vector<UINT> fullIndexData;

	UINT fVertexCount, fIndexCount;
	UINT fWidth, fHeight;

	int triangleCount, drawCount;
	MeshNodeData* data;
};