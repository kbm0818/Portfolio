#include "stdafx.h"
#include "Billboard.h"
#include "Executes/Execute.h"

Billboard::Billboard(wstring textureFile)
	: Renderer(), textureFile(textureFile)
	, scale(10.0f, 10.0f, 10.0f)
{
	CreateData();
	CreateBuffer();
	CreateTexture();
}

Billboard::~Billboard()
{
	SAFE_DELETE(worldBuffer);
	SAFE_RELEASE(texture);

	SAFE_DELETE(shader);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Billboard::Update(ExecuteValues* exValue)
{
	D3DXVECTOR3 camPos;
	exValue->MainCamera->GetPosition(&camPos);

	double angle = atan2(position.x - camPos.x, position.z - camPos.z) * (180.0 / D3DX_PI);
	float rotation = (float)angle * 0.0174532925f;

	D3DXMATRIX rotateMat, scaleMat;
	D3DXMatrixRotationY(&rotateMat, rotation);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	D3DXMatrixScaling(&scaleMat, scale.x, scale.y, scale.z);
	
	world = scaleMat * rotateMat * world;
}

void Billboard::Render()
{
	worldBuffer->SetMatrix(world);
	worldBuffer->SetVSBuffer(1);

	shader->Render(ShaderType::VP);

	D3D::GetDC()->PSSetShaderResources(0, 1, &texture);

	Renderer::Render();
}

void Billboard::SetPosition(D3DXVECTOR3 & pos)
{
	position = pos;
}

void Billboard::GetPosition(D3DXVECTOR3 & pos)
{
	pos = position;
}

void Billboard::CreateData()
{
	indexCount = vertexCount = 6;
	
	vertexData.resize(vertexCount);
	indexData.resize(indexCount);

	vertexData[0].position = D3DXVECTOR3(-0.5f, 0.0f, 0.0f);
	vertexData[0].uv = D3DXVECTOR2(0.0f, 1.0f);
	vertexData[1].position = D3DXVECTOR3(-0.5f, 1.0f, 0.0f);
	vertexData[1].uv = D3DXVECTOR2(0.0f, 0.0f);
	vertexData[2].position = D3DXVECTOR3(0.5f, 0.0f, 0.0f);
	vertexData[2].uv = D3DXVECTOR2(1.0f, 1.0f);
	vertexData[3].position = D3DXVECTOR3(0.5f, 0.0f, 0.0f);
	vertexData[3].uv = D3DXVECTOR2(1.0f, 1.0f);
	vertexData[4].position = D3DXVECTOR3(-0.5f, 1.0f, 0.0f);
	vertexData[4].uv = D3DXVECTOR2(0.0f, 0.0f);
	vertexData[5].position = D3DXVECTOR3(0.5f, 1.0f, 0.0f);
	vertexData[5].uv = D3DXVECTOR2(1.0f, 0.0f);

	for (UINT i = 0; i < 6; i++)
	{
		indexData[i] = i;
	}

	shader = new Shader(Shaders + L"Billboard.hlsl", ShaderType::VP);
	worldBuffer = new WorldBuffer();
}

void Billboard::CreateTexture()
{
	Texture::LoadTexture(textureFile, &texture);
}
