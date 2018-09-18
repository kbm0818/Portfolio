#pragma once

class Frustum;
class TerrainQuadTree
{
public:
	TerrainQuadTree();
	~TerrainQuadTree();

	virtual void Update(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);
	virtual void ResizeScreen(GlobalValues* exValue);

	void Init(GlobalValues* exValue);

private:
	typedef VertexColorTexture2NormalTangentBinormal VertexType;

	struct NodeType
	{
		float positionX, positionZ, width;
		int triangleCount;
		ID3D11Buffer *vertexBuffer, *indexBuffer;
		NodeType* nodes[4];
	};

	void CreateFullData(GlobalValues* exValue);
	void CreateQuadTreeData(GlobalValues* exValue);

	void CalculateMeshDimensions(int vertexCount, float &centerX, float &centerZ, float &meshWidth);
	void CreateTreeNode(NodeType *node, float positionX, float positionZ, float width);
	int CountTriangles(float positionX, float positionZ, float width);
	bool IsTriangleContained(int index, float positionX, float positionZ, float width);
	void ReleaseNode(NodeType *node);
	void RenderNode(NodeType *node, Frustum *frustum);

private:
	vector<VertexType> fullVertexData;
	vector<UINT> fullIndexData;

	UINT fVertexCount, fIndexCount;
	UINT fWidth, fHeight;

	int triangleCount, drawCount;
	NodeType* parentNode;

	vector<class IComponent*> components;
};