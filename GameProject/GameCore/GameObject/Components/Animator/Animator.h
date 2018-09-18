#pragma once
#include "../ComponentBase.h"

class ModelAnimationController;
class ModelSkeleton;
class Animator : public ComponentBase
{
public:
	Animator();
	virtual ~Animator();

	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void Render();

	ShaderBuffer* GetBoneBuffer() { return boneBuffer; }
	ModelAnimationController* GetAnimationController() { return controller; }
	ModelSkeleton* const GetSkeleton() { return skeleton; }
	void CopySkeleton(ModelSkeleton* source);

	struct Data
	{
		D3DXMATRIX boneArray[100];
	};

private:
	ModelAnimationController* controller;
	ModelSkeleton * skeleton;

	void SetBoneArray(D3DXMATRIX* matrix, UINT count);
	void GetBoneMatrix(int index, D3DXMATRIX* mat);

	ShaderBuffer* boneBuffer;
	Data data;
};