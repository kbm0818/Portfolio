#pragma once

class MoModel;
class MoMaterial;
class MoBuffer;
class MoBoneWeights;
class Model;
class MoLoader
{
public:
	static void Convert(string fbxFile, string saveFile);
	static void LoadBinary(string binFile, Model** model);
	static void WriteBinary(string binFile, Model* model);
	static void AddAnimation(string fbxAnimFile, Model** model, string animName);
	static void AddModel(string binFile, Model** model, string boneName);

private:
	MoLoader();
	~MoLoader();

	MoModel* Load(string file);
	void LoadAnim(string file, Model** model, string animName);
	void LoadPart(string file, Model** model);

	void ProcessMaterial();
	void ProcessNode(FbxNode* node, FbxNodeAttribute::EType type);
	void ProcessMesh(FbxNode* node);

	void ProcessSkeleton(FbxNode * node);
	void ProcessBoneWeights(FbxMesh* mesh, vector<MoBoneWeights>& meshBoneWeights);
	void ProcessBoneWeights(FbxSkin* skin, vector<MoBoneWeights>& meshBoneWeights);
	void ProcessAnimations(Model** model,string animName);
	void ProcessAnimation(Model** model, FbxNode * node, wstring takeName, float frameRate, float start, float stop);

	D3DXMATRIX GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time);
	D3DXMATRIX GetLocalTransformFromCurrentTake(FbxNode * node, FbxTime time);

	MoMaterial* LinkMaterialWithPolygon
	(
		FbxMesh* mesh, int layerIndex
		, int polygonIndex, int polygonVertexIndex
		, int vertexIndex
	);

	int GetMappingIndex
	(
		FbxLayerElement::EMappingMode mode
		, int polygonIndex
		, int polygonVertexIndex
		, int vertexIndex
	);
	
	D3DXVECTOR2 GetUV
	(
		FbxMesh* mesh, int layerIndex
		, int polygonIndex, int polygonVertexIndex
		, int vertexIndex
	);

	MoMaterial* GetMaterial(FbxSurfaceMaterial* fbxMaterial);
	D3DXMATRIX GetGeometricOffset(FbxNode* node);

	MoModel* model;

	FbxManager* manager;
	FbxScene* scene;
	FbxImporter* importer;
	FbxGeometryConverter* converter;

	vector<FbxSurfaceMaterial *> materials;
};
