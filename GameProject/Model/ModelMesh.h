#pragma once

class Model;
class ModelMaterial;
class ModelBuffer;
class ModelBoneWeights;
class OptionBuffer;
class MaterialBuffer;
struct MeshData;
class ModelMesh
{
public:
	ModelMesh(UINT materialIndex);
	~ModelMesh();

	void CreateBuffer(VertexTextureNormalBlend* vertex, UINT* index);
	const UINT GetVertexCount() { return vertexCount; }
	const UINT GetIndexCount() { return indexCount; }
	const UINT GetJointIndex() { return jointIndex; }
	const UINT GetMaterialIndex() { return materialIndex; }

	bool IsSkinning() { return isSkinnedModel; }

	void MoveMeshData(MeshData* data);

private:
	friend class ModelLoader;

	ID3D11Buffer* vertexBuffer;
	UINT vertexCount;
	UINT vertexStride;

	ID3D11Buffer* indexBuffer;
	UINT indexCount;

	UINT materialIndex;
	int jointIndex;
	bool isSkinnedModel;
};
