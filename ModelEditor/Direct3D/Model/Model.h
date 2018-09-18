#pragma once

class ModelMaterial;
class ModelPart;
class ModelSkeleton;
class ModelAnimationController;
class AnimBuffer;
class Shader;
class Model
{
public:
	~Model();

	D3DXMATRIX GetGeometricOffset() { return matGeometricOffset; }
	ModelMaterial* GetMatchMaterial(UINT number);
	
	ModelMaterial* GetMaterial(UINT number)
	{
		return materials[number];
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

	void SetScale(D3DXVECTOR3 scale);
	void SetScale(float x, float y, float z);
	void SetScale(float scale);

	void UpdateScale();

	void Update();
	void Render(Camera* camera);

private:
	friend class MoModel;
	friend class MoLoader;
	friend class MoPart;

	Model();

	D3DXMATRIX matGeometricOffset;
	D3DXVECTOR3 scale;

	vector<ModelMaterial *> materials;
	vector<ModelPart *> parts;

	AnimBuffer* buffer;

	ModelSkeleton* skeleton;
	ModelAnimationController* animationController;
};