#pragma once
#include <vector>
#include <unordered_map>

class Material;
class ModelSkeleton;
class ModelAnimation;
class Mesh;
class Collider;
class Shader;

class ModelData
{
public:
	ModelData() {}
	~ModelData() {}

	struct MeshInfo
	{
		UINT rootBoneIndex;
		bool isSkinning;
		UINT materialIndex;
	};
private:
	friend class AssetManager;

	vector<pair<MeshInfo,Mesh*>> meshes;
	ModelSkeleton* skeleton;
	vector<Material*> embeddedMaterials;
	vector<pair<int,Collider*>> embeddedColliders;
	vector<ModelAnimation*> embeddedAnimations;
};

class AssetManager
{
public:
	static AssetManager* Get();

	static void Create();
	static void Delete();

	void AddModel(string name, string filePath);
	template <typename S, typename V>
	void AddShader(string name, wstring filePath);

	void AddMesh(string name, Mesh* mesh);
	void AddMaterial(string name, wstring diffuse = L"", wstring specular = L"", wstring ambient = L"", wstring emissive = L"",
		D3DXCOLOR diffuseColor = D3DXCOLOR(1, 1, 1, 1), D3DXCOLOR specularColor = D3DXCOLOR(1, 1, 1, 1), D3DXCOLOR ambientColor = D3DXCOLOR(1, 1, 1, 1), D3DXCOLOR emissiveColor = D3DXCOLOR(1,1,1,1), int shininess = 10);

	Shader* GetShader(string name);
	Mesh* GetMesh(string name);
	Material* GetMaterial(string name);

	void SetupToLoadedModel(string name, GameObject** object);
	void SetupMesh(string mesh, string material, GameObject** object);

private:
	AssetManager();
	~AssetManager();

	static AssetManager* instance;

	unordered_map<string, ModelData*> modelContainer;
	unordered_map<string, Shader*> shaderContainer;

	unordered_map<string, Material*> materialContainer;
	unordered_map<string, ModelAnimation*> animationContainer;
	unordered_map<string, Mesh*> meshContainer;
};


template <typename S, typename V>
void AssetManager::AddShader(string name, wstring filePath)
{
	S* shader = new S(filePath);
	shader->CreateInputLayout(V::desc, V::count);
	shaderContainer.insert(make_pair(name, shader));
}