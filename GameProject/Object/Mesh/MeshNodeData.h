#pragma once
#include "MeshData.h"

struct MeshNode
{
	float positionX, positionZ, width;
	int triangleCount;
	MeshData<VertexTextureNormalTangentBinormal>* mesh;
	MeshNode* nodes[4];
};

class Frustum;
class MeshNodeData : public IMeshData
{
public:
	MeshNodeData();
	~MeshNodeData();

	virtual void Update(GlobalValues* values);
	virtual void Draw(GlobalValues* values, D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual void SetDrawOption(int op) { meshRenderOption = op; };

	void RenderNode(MeshNode* node, GlobalValues* values);
	void ReleaseNode(MeshNode* node);

	void OnOption(GlobalValues* values);
	void OffOption(GlobalValues* values);

private:
	friend class TerrainMeshFactory;
	MeshNode* node;

	unsigned int meshRenderOption;
};