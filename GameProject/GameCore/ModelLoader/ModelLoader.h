#pragma once

class ModelScene;
class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	//static void Read(string file, ModelScene** modelScene);

private:
	friend class AssetManager;

	static void ReadMaterial(BinaryReader * r, class ModelMaterial* modelMaterial);
	static void ReadModel(BinaryReader* r, class Model* model);
	static void ReadModelMesh(BinaryReader* r, class ModelMesh* modelMesh);

	//Skeleton, Animation
	static void ReadSkeleton(BinaryReader * r, class ModelSkeleton * skeleton, vector<class ModelAnimation*>& animationClips);
	static void ReadKeyFrames(BinaryReader * r, class ModelAnimationKeyFrames * dest);

	static void ReadAnimation(BinaryReader * r, class ModelAnimationController * animationController);

	//Colliders
	static void ReadCollider(BinaryReader * r, class Collider ** collider);
	static void ReadCapsuleCollider(BinaryReader * r, Collider ** collider);
};
