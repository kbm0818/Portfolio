#pragma once

class ModelMaterial;
class ModelPart;
class ModelSkeleton;
class ModelAnimationController;
class ModelBuffer;
class Shader;
class Model : public IMeshData
{
public:
	friend class FbxLoader;

	Model();
	virtual ~Model();

	virtual void Update(GlobalValues* values);
	virtual void Draw(GlobalValues* values, D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual void SetDrawOption(int op) { meshRenderOption = op; }

	void Load(wstring modelName, GlobalValues* values);

	D3DXMATRIX GetGeometricOffset();
	ModelMaterial* GetMatchMaterial(UINT number);
	
	ModelMaterial* GetMaterial(UINT number)
	{
		if (number < materials.size())
			return materials[number];
		else
			return nullptr;
	}

	UINT GetMaterialCount() { return materials.size(); }
	ModelSkeleton* GetSkeleton() { return skeleton; }

	UINT GetModelPartSize() { return parts.size(); }
	string GetModelPartName(UINT index);

	bool DeleteModelPart(UINT index);

	void DeleteAnimation(UINT index);
	void PlayAnimation(UINT index);

	ModelAnimationController* GetAnimationController() { return animationController; }

	ModelPart* GetModelPart(string name);

	void Update();
	void Render(struct GlobalValues* exValue);

private:
	vector<ModelMaterial *> materials;
	vector<ModelPart *> parts;

	ModelSkeleton* skeleton;
	ModelAnimationController* animationController;

	unsigned int meshRenderOption;
};