#pragma once

class ModelMesh;
class ModelAnimationKeyFrames;
class ModelAnimationController;
class ModelSkeletonBone;
class ModelSkeleton;
class ModelBuffer;

class Model
{
public:
	Model(wstring name);
	~Model();

	wstring GetName() { return name; }
	vector<ModelMesh*>& GetPartsList() { return parts; }

private:
	friend class ModelLoader;

	wstring name;
	vector<ModelMesh* > parts;
};