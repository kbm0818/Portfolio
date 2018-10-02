#pragma once

class ModelMaterial;
class ModelPart;
class ModelBuffer;
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

	ModelMesh(ModelPart* modelPart);
	~ModelMesh();

	void Render(struct GlobalValues* exValue);

	void Update(bool isAnimation);
	void Load(BinaryReader* r, GlobalValues* values);

	void CreateBuffer();

	ModelMaterial* GetMaterial()
	{
		return material;
	}

	ModelPart* modelPart;

	ModelMaterial* material;

	typedef VertexTextureNormalTangentBinormalBlend VertexType;

	VertexType* vertexData;
	UINT* indexData;

	ID3D11Buffer* vertexBuffer, *indexBuffer;
	UINT vertexCount, indexCount;

	bool isSkinnedModel;

	D3DXMATRIX world;
	D3DXMATRIX defaultworld;

	class TransformC* transform;
};
