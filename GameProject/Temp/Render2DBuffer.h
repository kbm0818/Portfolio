#pragma once
#include "../Renders/ShaderBuffer.h"

class Render2DBuffer : public ShaderBuffer
{
public:
	Render2DBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		data.maskStart = { 0, 0 };
		data.maskSize = { 0, 0 };
		data.maskLight = { 300, 300 }; //ºûÀÇ ÁßÁ¡
		data.maskAmount = 1e-3f;
	}

	void SetMaskAmount(float val) { data.maskAmount = val; }
	void SetMaskSize(D3DXVECTOR2 size) { data.maskSize = size; }

	void SetBlur(D3DXVECTOR2 size, int count, float amount)
	{
		data.blurImageSize = size;
		data.blurCount = count;
		data.blurLaplaceAmount = amount;
	}

	struct Data
	{
		D3DXVECTOR2 maskStart;
		D3DXVECTOR2 maskSize;
		D3DXVECTOR2 maskLight;
		float maskAmount;
		float maskPadding;

		D3DXVECTOR2 blurImageSize;
		int blurCount;
		float blurLaplaceAmount;
	};

private:
	Data data;
};