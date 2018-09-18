#pragma once
#include "../Renders/ShaderBuffer.h"


class ModelBuffer : public ShaderBuffer
{
public:
	ModelBuffer()
		: ShaderBuffer(&data, sizeof(Data))
	{
		D3DXMatrixIdentity(&data.boneScale);
		//D3DXMatrixScaling(&data.boneScale, 0.01f, 0.01f, 0.01f);

		for (int i = 0; i < 100; i++)
			D3DXMatrixIdentity(&data.boneArray[i]);

		data.isSkinning = false;
		data.isWireFrame = false;
	}

	void SetSkinning(bool isSkinning)
	{
		data.isSkinning = (UINT)isSkinning;
	}
	void SetWireFrameMode(bool set)
	{
		data.isWireFrame = (set)? 1 : 0;
	}

	void SetBoneArray(D3DXMATRIX* matrix, UINT count)
	{
		memcpy(data.boneArray, matrix, count * sizeof(D3DXMATRIX));

		for (UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.boneArray[i], &data.boneArray[i]);
	}

	void GetBoneMatrix(int index, D3DXMATRIX* mat)
	{
		*mat = data.boneArray[index];
	}


	struct Data
	{
		D3DXMATRIX boneScale;
		D3DXMATRIX boneArray[100];
		UINT isSkinning;
		UINT isWireFrame;
		D3DXVECTOR2 padding;
	};

private:
	Data data;
};



