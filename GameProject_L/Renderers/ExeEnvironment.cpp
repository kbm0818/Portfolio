#include "stdafx.h"
#include "ExeEnvironment.h"
#include "../Environments/SkyPlane.h"
#include "../Environments/SkyPlane_Perlin.h"
#include "../Environments/LensFlare.h"
#include "../Environments/ReflectionWater.h"
#include "../Renders/RenderTarget.h"
#include "LandScape/Billboard.h"

ExeEnvironment::ExeEnvironment(ExecuteValues* values)
	: Execute(values)
	, bDrawSky(true), inWater(false)
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	refractTarget = new RenderTarget();
	reflectTarget = new RenderTarget();

	reflectShader = new Shader(Shaders + L"Reflection.hlsl", ShaderType::VP);
	//reflectShaderBuffer = new ReflectShaderBuffer();

	//values->GlobalLight->Data.Direction;
	skyPlane = new SkyPlane_Perlin();
	lensFlare = new LensFlare();
	water = new ReflectionWater();
	
	refractClip = D3DXVECTOR4(0.0f, -1.0f, 0.0f, water->GetWaterHeight() + 0.1f);
	reflectClip = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -water->GetWaterHeight());

	tree = new Billboard(Textures + L"tree.png");
	D3DXVECTOR3 treePosition = D3DXVECTOR3(50.0f, 0.0f, 90.0f);
	tree->SetPosition(treePosition);
}

ExeEnvironment::~ExeEnvironment()
{
	SAFE_DELETE(tree);
	SAFE_DELETE(reflectShader);
	SAFE_DELETE(reflectShaderBuffer);

	SAFE_DELETE(water);
	SAFE_DELETE(skyPlane);
	SAFE_DELETE(lensFlare);

	SAFE_DELETE(refractTarget);
	SAFE_DELETE(reflectTarget);
}

void ExeEnvironment::Update()
{
	if (bDrawSky == true)
	{
		skyPlane->Update(values->MainCamera);

		D3DXVECTOR3 camPos;
		values->MainCamera->GetPosition(&camPos);

		D3DXMATRIX projection;
		values->Perspective->GetMatrix(&projection);

		D3DXMATRIX view;
		values->MainCamera->GetMatrix(&view);

		D3DXVECTOR3 lightDirection;
		lightDirection = values->GlobalLight->Data.Direction;
		lightDirection *= -1.0f;

		lensFlare->Update(camPos, lightDirection, view, projection);

		water->Update(values);

		if (camPos.y < water->GetWaterHeight())
			inWater = true;
		else
			inWater = false;
	}
}

void ExeEnvironment::PreRender()
{
	D3D::Get()->SetRenderTarget(refractTarget->GetRTV());
	D3D::Get()->Clear(refractTarget->GetRTV());
	{
			RenderOptions::Get()->TurnOffZBuffer();
			RenderOptions::Get()->TurnOffCullMode();
		
			RenderOptions::Get()->TurnOnCullMode();
			RenderOptions::Get()->TurnOnAlphaBlending();
			skyPlane->Render();
			RenderOptions::Get()->TurnOnZBuffer();
			//lensFlare->Render();
			RenderOptions::Get()->TurnOffAlphaBlending();
			//if(!inWater)
			//	reflectShaderBuffer->Data.clipPlane = refractClip;
			//else
			//	reflectShaderBuffer->Data.clipPlane = reflectClip;
			//reflectShaderBuffer->SetVSBuffer(2);
	}

	D3D::Get()->SetRenderTarget(reflectTarget->GetRTV());
	D3D::Get()->Clear(reflectTarget->GetRTV(),D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	{
		values->MainCamera->UpdateReflectionView(water->GetWaterHeight());
		D3DXMATRIX mat;
		values->MainCamera->GetReflectionMatrix(&mat);
		values->ViewProjection->SetView(mat);
		values->ViewProjection->SetVSBuffer(0);
		D3DXVECTOR3 camPos;
		values->MainCamera->GetPosition(&camPos);
		camPos.y = -camPos.y + (water->GetWaterHeight() * 2.0f);
		D3DXMatrixTranslation(&mat, camPos.x, camPos.y, camPos.z);

		
		RenderOptions::Get()->TurnOffZBuffer();
		RenderOptions::Get()->TurnOffCullMode();
		RenderOptions::Get()->TurnOnCullMode();
		RenderOptions::Get()->TurnOnAlphaBlending();
		skyPlane->Render(&mat);
		RenderOptions::Get()->TurnOnZBuffer();
		//lensFlare->Render();
		RenderOptions::Get()->TurnOffAlphaBlending();
		/*if (!inWater)
			reflectShaderBuffer->Data.clipPlane = reflectClip;
		else
			reflectShaderBuffer->Data.clipPlane = refractClip;
		reflectShaderBuffer->SetVSBuffer(2);*/

		tree->Update(values);
	}

	if (Keyboard::Get()->Down(VK_F1))
	{
		refractTarget->SaveTexture(L"refract.png");
	}
	if (Keyboard::Get()->Down(VK_F2))
	{
		reflectTarget->SaveTexture(L"reflect.png");
	}
}

void ExeEnvironment::Render()
{
	if (bDrawSky == true)
	{
		RenderOptions::Get()->TurnOffZBuffer();
		RenderOptions::Get()->TurnOffCullMode();#include "stdafx.h"
#include "String.h"

//////////////////////////////////////////////////////////////////////////
///@brief 문자열 자르기
///@param orgin : 원본 문자열
///@param tok : 자를 기준이 되는 문자열
///@return 완료된 문자열 배열
//////////////////////////////////////////////////////////////////////////
void String::SplitString(vector<string>* result, string origin, string tok)
{
	result->clear();

	int cutAt = 0; //자를 위치s
	while ((cutAt = (int)origin.find_first_of(tok)) != origin.npos)
	{
		if (cutAt > 0) //자르는 위치가 0보다크면
			result->push_back(origin.substr(0, cutAt));

		origin = origin.substr(cutAt + 1);
	}

	if (origi