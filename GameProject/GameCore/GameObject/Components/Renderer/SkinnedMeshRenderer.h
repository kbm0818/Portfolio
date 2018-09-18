#pragma once
#include "Renderer.h"

class SkinnedMeshRenderer : public Renderer
{
public:
	SkinnedMeshRenderer();
	virtual ~SkinnedMeshRenderer()
	{
		SAFE_DELETE(buffer);
	}

	void Init(Material* material, ShaderBuffer* boneBuffer, UINT rootBone, bool skinning);

	virtual void Awake();
	virtual void Start() {}
	virtual void Update();
	virtual void Render();

	struct Data
	{
		//���������� Bone�� ���� Animation �Ǵ� mesh�� �ƴ� ��� (isSkinning�� false)
		//�׳� rootBoneIndex�� ��ġ�� ���õǼ� �������ȴ�.
		float isSkinning;
		float rootBoneIndex;
		D3DXVECTOR2 padding;
	};
private:
	ShaderBuffer* buffer;
	ShaderBuffer* boneBuffer;
	Data data;
};