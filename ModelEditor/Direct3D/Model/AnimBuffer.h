#pragma once
#include "../Shaders/ShaderBuffer.h"

class AnimBuffer : public ShaderBuffer
{
public:
	AnimBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		//D3DXMatrixIdentity(&data.boneScale);
		D3DXMatrixScaling(&data.boneScale, 0.1f, 0.1f, 0.1f);

		for (int i = 0; i < 100; i++)
			D3DXMatrixIdentity(&data.boneArray[i]);

		data.isSkinning = false;
	}

	void SetSkinning(bool isSkinning)
	{
		data.isSkinning = (UINT)isSkinning;
	}

	void SetBoneArray(D3DXMATRIX* matrix, UINT count)
	{
		memcpy(data.boneArray, matrix, count * sizeof(D3DXMATRIX));

		for (UINT i = 0; i < count; i++)
			D3DXMatrixTranspose(&data.boneArray[i], &data.boneArray[i]);
	}

	void SetBoneScale(D3DXMATRIX matrix)
	{
		data.boneScale = matrix;
	}

	D3DXMATRIX GetBoneScale() { return data.boneScale; }

	struct Data
	{
		D3DXMATRIX boneScale;
		D3DXMATRIX boneArray[100];
		UINT isSkinning;
		D3DXVECTOR3 padding;
	};

private:
	Data data;
};