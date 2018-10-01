#pragma once

class FbxMaterialData;
class FbxBoneWeightsData;
class FbxModelData;
class Model;
class FbxLoader
{
public:
	static void Convert(wstring fbxFile, wstring saveFile = L"");
	static void ConvertAnim(wstring fbxFile);

	static void AddAnimation(string fbxAnimFile, Model** model, string animName);

	static IMeshData* Load(wstring modelName, GlobalValues* values);
	static bool LoadAnim(wstring animName, Model* model, bool rootMotion = false, bool repeatMotion = true);

private:
	FbxLoader();
	~FbxLoader();

	void LoadAnim(string file, Model** model, string animName);
	void ProcessAnimations(Model** model, string animName);
	void ProcessAnimation(Model** model, FbxNode * node, wstring takeName, float frameRate, float start, float stop);

	FbxModelData* Load(string file);
	FbxModelData* Load(wstring file);

	void LoadAnim(BinaryWriter* w, string file, string animName);
	//void LoadPart(string file, Model** model);

	void ProcessMaterial();
	void ProcessNode(FbxNode* node, FbxNodeAttribute::EType type);
	void ProcessMesh(FbxNode* node);

	void ProcessSkeleton(FbxNode * node);
	void ProcessBoneWeights(FbxMesh* mesh, vector<FbxBoneWeightsData>& meshBoneWeights);
	void ProcessBoneWeights(FbxSkin* skin, vector<FbxBoneWeightsData>& meshBoneWeights);
	
	void ProcessAnimations(BinaryWriter* w, string animName);
	void ProcessAnimation(BinaryWriter* w, FbxNode * node, wstring takeName, float frameRate, float start, float stop);

	D3DXMATRIX GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time);
	D3DXMATRIX GetLocalTransformFromCurrentTake(FbxNode * node, FbxTime time);

	FbxMaterialData* LinkMaterialWithPolygon
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

	FbxMaterialData* GetMaterial(FbxSurfaceMaterial* fbxMaterial);
	D3DXMATRIX GetGeometricOffset(FbxNode* node);

	FbxModelData* model;

	FbxManager* manager;
	FbxScene* scene;
	FbxImporter* importer;
	FbxGeometryConverter* converter;

	vector<FbxSurfaceMaterial *> materials;
};
