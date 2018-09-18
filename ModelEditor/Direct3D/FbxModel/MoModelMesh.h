#pragma once

class MoModel;
class MoMaterial;
class Model;
class ModelMesh;
class MoPart;
class MoBoneWeights;
class MoModelMesh
{
public:
	MoModelMesh(MoPart* part, MoMaterial* material);
	~MoModelMesh();

	MoMaterial* GetMaterial() { return material; }

	void AddVertex(D3DXVECTOR3& position, D3DXVECTOR3& normal, D3DXVECTOR2& uv, const MoBoneWeights& boneWeight);
	void AddVertex(D3DXVECTOR3& position, D3DXVECTOR3& normal, D3DXVECTOR2& uv);

	void Write(BinaryWriter* w);
	static void Read(BinaryReader* r, Model* model, ModelMesh* modelMesh);
	static void Write(BinaryWriter* w, ModelMesh* modelMesh);
	
private:
	void CalcTBB();

	friend class MoLoader;
	friend class MoModel;

	MoPart* part;
	MoModel* model;
	MoMaterial* material;

	typedef VertexTextureNormalTangentBinormalBlend VertexType;

	// ������ �ֵ�;
	vector<VertexType> vertices;
	vector<UINT> indices;
	vector<MoBoneWeights> boneWeights;

	bool isSkinnedModel;

	//��� ���� �׸��갡 �ƴϴ� ���۸� �����ʿ���� vertices�� indices, boneWeights�� ������ ������ �ȴ�
};
