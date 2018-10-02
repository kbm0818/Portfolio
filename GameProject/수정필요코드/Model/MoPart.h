#pragma once

class MoModelMesh;
class MoMaterial;
class MoAnimationKeyFrames;
class MoBoneWeights;
class ModelPart;
class Model;
class MoPart
{
public:
	MoPart(string name);
	MoPart();
	~MoPart();

	void AddVertex(MoMaterial*, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, const MoBoneWeights&);
	void AddVertex(MoMaterial*, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2);
	
	void SetGeometricOffset(const D3DXMATRIX& matGeometricOffset)
	{
		this->matGeometricOffset = matGeometricOffset;
	}

	D3DXMATRIX GetGeometricOffset()
	{
		return matGeometricOffset;
	}

	D3DXMATRIX GetAnimationTransform()
	{
		return matAnimationTransform;
	}

	void SetAbsoluteTransform(const D3DXMATRIX& matAbsoluteTransform)
	{
		this->matAnimationTransform = matAbsoluteTransform;
	}

	void Write(BinaryWriter* w);
	static void Read(BinaryReader* r, Model* model, ModelPart* part);
	static void Write(BinaryWriter* w, ModelPart* modelMesh);

private:
	//요넘들은 저장해야함
	string name; 
	vector<MoModelMesh *> meshes;

	//typedef pair<string, MoAnimationKeyFrames*> Pair;
	//vector<Pair> animationKeyFrames;

	D3DXMATRIX matGeometricOffset;
	D3DXMATRIX matAnimationTransform;

};