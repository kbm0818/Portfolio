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
		//직접적으로 Bone에 의해 Animation 되는 mesh가 아닌 경우 (isSkinning이 false)
		//그냥 rootBoneIndex의 위치로 셋팅되서 렌더링된다.
		float isSkinning;
		float rootBoneIndex;
		D3DXVECTOR2 padding;
	};
private:
	ShaderBuffer* buffer;
	ShaderBuffer* boneBuffer;
	Data data;
};