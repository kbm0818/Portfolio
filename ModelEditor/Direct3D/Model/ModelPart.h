#pragma once

class ModelAnimationKeyFrames;
class ModelAnimationController;
class ModelAnimation;
class AnimBuffer;
class ModelMesh;
class ModelSkeletonBone;
class ModelPart
{
public:
	ModelPart(string name, AnimBuffer* modelBuffer);
	ModelPart(AnimBuffer* modelBuffer);
	~ModelPart();
	
	void Update(bool isAnimation);
	void UpdateAnimation(ModelAnimationController* animationController);
	void Render(Camera* camera);

	string GetName() { return name; }
	void AddAnimationKeyFrames(ModelAnimationKeyFrames* animationKeyFrames);
	ModelAnimationKeyFrames* GetAnimationKeyFrames(wstring animationName);

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

	void SetModelSkeletonBone(ModelSkeletonBone* skeleton) { this->modelSkeletonBone = skeleton; }
	void SetEquip(bool e) { this->isEquip = e; }

private:
	friend class MoPart;
	friend class MoModel;
	friend class MoLoader;
	//요넘들은 저장해야함
	string name; 
	vector<ModelMesh *> meshes;

	typedef pair<wstring, ModelAnimationKeyFrames*> Pair;
	vector<Pair> animationKeyFrames;

	AnimBuffer* buffer;

	D3DXMATRIX matGeometricOffset;
	D3DXMATRIX matAnimationTransform;

	bool isEquip;
	ModelSkeletonBone* modelSkeletonBone;

	Model* model;
	wstring boneName;
};