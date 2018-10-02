#pragma once
#include "stdafx.h"
#include "Object\Shader\ShaderBuffer.h"

class WorldBuffer : public ShaderBuffer
{
public:
	WorldBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.World);
	}

	void SetMatrix(D3DXMATRIX mat)
	{
		Data.World = mat;
		D3DXMatrixTranspose(&Data.World, &Data.World);
	}

	void SetMatrixNonTranspose(D3DXMATRIX mat)
	{
		Data.World = mat;
	}

	struct Struct
	{
		D3DXMATRIX World;
	};

private:
	Struct Data;
};

class ViewProjectionBuffer : public ShaderBuffer
{
public:
	ViewProjectionBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		D3DXMatrixIdentity(&Data.View);
		D3DXMatrixIdentity(&Data.Projection);
		D3DXMatrixIdentity(&Data.Reflection);
	}

	void SetView(D3DXMATRIX mat)
	{
		Data.View = mat;
		D3DXMatrixTranspose(&Data.View, &Data.View);
	}

	void SetProjection(D3DXMATRIX mat)
	{
		Data.Projection = mat;
		D3DXMatrixTranspose(&Data.Projection, &Data.Projection);
	}

	void SetReflection(D3DXMATRIX mat)
	{
		Data.Reflection = mat;
		D3DXMatrixTranspose(&Data.Reflection, &Data.Reflection);
	}

	struct Struct
	{
		D3DXMATRIX View;
		D3DXMATRIX Projection;
		D3DXMATRIX Reflection;
	};

//private:
	Struct Data;
};

class GlobalLightBuffer : public ShaderBuffer
{
public:
	GlobalLightBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.Direction = D3DXVECTOR3(-1, -1, 1);
	}

	struct Struct
	{
		D3DXVECTOR3 Direction;
		float Padding;
	};

	Struct Data;
};

class GlobalCameraBuffer : public ShaderBuffer
{
public:
	GlobalCameraBuffer() : ShaderBuffer(&Data, sizeof(Struct))
	{
		Data.position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	struct Struct
	{
		D3DXVECTOR3 position;
		float Padding;
	};

	Struct Data;
};