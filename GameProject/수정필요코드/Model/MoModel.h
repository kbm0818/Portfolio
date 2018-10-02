#pragma once

class MoMaterial;
class MoModelMesh;
class Model;
class MoSkeleton;
class MoAnimationController;
class MoBoneWeights;
class MoPart;
class MoModel
{
private:
	friend class MoLoader;

	MoModel(string file);
	~MoModel();

	void PushMaterial(MoMaterial* material);

	D3DXMATRIX GetAbsoluteTransformFromCurrentTake(FbxNode* node, FbxTime time);

	void Write(string file);
	static void Read(string file, Model** model);
	static void ReadParts(string file, Model** model);
	static void Write(string file, Model* model);

	string file;

	D3DXMATRIX geometricOffset;

	MoSkeleton* skeleton;

	vector<MoMaterial *> materials;
	vector<MoPart* > parts;
};