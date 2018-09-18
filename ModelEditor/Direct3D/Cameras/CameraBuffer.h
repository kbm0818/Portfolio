#pragma once
#include "../Shaders/ShaderBuffer.h"

class CameraBuffer : public ShaderBuffer
{
public:
	CameraBuffer() : ShaderBuffer(&data, sizeof(Data))
	{
		D3DXMatrixIdentity(&data.view);
		D3DXMatrixIdentity(&data.projection);
	}

	void SetWorld(D3DXMATRIX* world)
	{
		memcpy(&data.world, world, sizeof(D3DXMATRIX));
		D3DXMatrixTranspose(&data.world, &data.world);
	}

	void SetView(D3DXMATRIX* view)
	{
		memcpy(&data.view, view, sizeof(D3DXMATRIX));
		D3DXMatrixTranspose(&data.view, &data.view);
	}

	void SetProjection(D3DXMATRIX* projection)
	{
		memcpy(&data.projection, projection, sizeof(D3DXMATRIX));
		D3DXMatrixTranspose(&data.projection, &data.projection);
	}

	void SetCameraPosition(D3DXVECTOR3 position)
	{
		memcpy(&data.position, position, sizeof(D3DXVECTOR3));
	}

	struct Data
	{		
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;

		D3DXVECTOR3 position;
		float padding;
	};

private:
	Data data;
};