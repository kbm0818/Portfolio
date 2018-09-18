#pragma once

class ModelMaterial;
class ModelPart;
class AnimBuffer;
class ModelBoneWeights;
class Shader;
class ModelMesh
{
public:
	void SetMaterial(ModelMaterial* material) { this->material = material; }

private:
	friend class MoModel;
	friend class MoModelMesh;
	friend class MoPart;
	friend class ModelPart;

	ModelMesh(ModelPart* modelPart, AnimBuffer* modelBuffer);
	~ModelMesh();

	void Update(bool isAnimation);
	void Render(Camera* camera);

	void CreateBuffer();

	ModelMaterial* GetMaterial()
	{
		return material;
	}

	ModelPart* modelPart;

	ModelMaterial* material;
	AnimBuffer* modelBuffer;

	typedef VertexTextureNormalTangentBinormalBlend VertexType;

	VertexType* vertexData;
	UINT* indexData;

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	UINT vertexCount, indexCount;

	bool isSkinnedModel;

	D3DXMATRIX world;
};
