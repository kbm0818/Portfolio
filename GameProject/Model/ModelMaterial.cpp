#include "stdafx.h"
#include "ModelMaterial.h"
#include "ModelBuffer.h"
#include "GameCore\Resource\Material.h"

ModelMaterial::ModelMaterial()
	: number(-1)
	, ambientFile(L""), emissiveFile(L""), diffuseFile(L""), specularFile(L"")
	, ambientView(NULL), diffuseView(NULL), specularView(NULL), emissiveView(NULL)
{

}

ModelMaterial::ModelMaterial(ModelMaterial & copy)
{
	number = copy.number;
	ambient = copy.ambient;
	emissive = copy.emissive;
	diffuse = copy.diffuse;
	specular = copy.specular;
	shininess = copy.shininess;

	ambientView = NULL;
	diffuseView = NULL;
	specularView = NULL;
	emissiveView = NULL;

	CreateView(AMBIENT, copy.ambientFile, &ambientView);
	CreateView(EMISSIVE, copy.emissiveFile, &emissiveView);
	CreateView(DIFFUSE, copy.diffuseFile, &diffuseView);
	CreateView(SPECULAR, copy.specularFile, &specularView);
}

ModelMaterial::~ModelMaterial()
{
	SAFE_RELEASE(ambientView);
	SAFE_RELEASE(emissiveView);
	SAFE_RELEASE(diffuseView);
	SAFE_RELEASE(specularView);
}


Material * ModelMaterial::MoveMaterial()
{
	Material* material = new Material;
	material->ambient = ambient;
	material->emissive = emissive;
	material->diffuse = diffuse;
	material->specular = specular;
	material->shininess = shininess;

	material->ambientView = ambientView;
	material->diffuseView = diffuseView;
	material->specularView = specularView;
	material->emissiveView = emissiveView;

	ambientView = nullptr;
	diffuseView = nullptr;
	specularView = nullptr;
	emissiveView = nullptr;

	return material;
}

void ModelMaterial::CreateView(MaterialType type, wstring path, ID3D11ShaderResourceView** view)
{
	if (path.length() < 1)
	{
		path = Contents + L"Textures/empty.png";
	}

	if (String::Contain(path, L"\\") == true)
		String::Replace(&path, L"\\", L"/");

	wstring filePath = L"";
	String::SplitFilePath(path, NULL, &filePath);
	filePath = path;

	switch (type)
	{
	case ModelMaterial::AMBIENT:
		ambientFile = filePath;
		break;
	case ModelMaterial::DIFFUSE:
		diffuseFile = filePath;
		break;
	case ModelMaterial::SPECULAR:
		specularFile = filePath;
		break;
	case ModelMaterial::EMISSIVE:
		emissiveFile = filePath;
		break;
	}

	HRESULT hr;

	DWORD fileValue = GetFileAttributes(filePath.c_str());
	if (fileValue < 0xFFFFFFFF)
	{
		D3DX11_IMAGE_INFO imageInfo;
		hr = D3DX11GetImageInfoFromFile(filePath.c_str(), NULL, &imageInfo, NULL);
		assert(SUCCEEDED(hr));

		D3DX11_IMAGE_LOAD_INFO loadInfo;
		if (imageInfo.Width > 512) loadInfo.Width = 512;
		if (imageInfo.Height > 512) loadInfo.Height = 512;

		hr = D3DX11CreateShaderResourceViewFromFile
		(
			D3D::GetDevice()
			, filePath.c_str()
			, &loadInfo
			, NULL
			, view
			, NULL
		);
		assert(SUCCEEDED(hr));


		//이미지 크기 검증 코드
		ID3D11Texture2D* texture;
		(*view)->GetResource((ID3D11Resource **)&texture);

		D3D11_TEXTURE2D_DESC desc;
		texture->GetDesc(&desc);
	}

	//DWORD fileValue = GetFileAttributes(filePath.c_str());
	//if (fileValue < 0xFFFFFFFF)
	//{
	//	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	//	(
	//		D3D::GetDevice()
	//		, filePath.c_str()
	//		, NULL
	//		, NULL
	//		, view
	//		, NULL
	//	);
	//	assert(SUCCEEDED(hr));

	//	return;
	//}

	//// Embedded Texture File이 있나 검사
	//fileValue = GetFileAttributes(path.c_str());
	//if (fileValue < 0xFFFFFFFF)
	//{
	//	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	//	(
	//		D3D::GetDevice()
	//		, path.c_str()
	//		, NULL
	//		, NULL
	//		, view
	//		, NULL
	//	);
	//	assert(SUCCEEDED(hr));

	//	return;
	//}
}

void ModelMaterial::SetAmbientTexture(wstring file)
{
	SAFE_RELEASE(ambientView);
	CreateView(AMBIENT, file, &ambientView);
}

void ModelMaterial::SetDiffuseTexture(wstring file)
{
	SAFE_RELEASE(diffuseView);
	CreateView(DIFFUSE, file, &diffuseView);
}

void ModelMaterial::SetSpecularTexture(wstring file)
{
	SAFE_RELEASE(specularView);
	CreateView(SPECULAR, file, &specularView);
}

void ModelMaterial::SetEmissiveTexture(wstring file)
{
	SAFE_RELEASE(emissiveView);
	CreateView(EMISSIVE, file, &emissiveView);
}

void ModelMaterial::SetAmbient(D3DXCOLOR & color)
{
	ambient = color;
}

void ModelMaterial::SetDiffuse(D3DXCOLOR & color)
{
	diffuse = color;
}

void ModelMaterial::SetSpecular(D3DXCOLOR & color)
{
	specular = color;
}

void ModelMaterial::SetEmissive(D3DXCOLOR & color)
{
	emissive = color;
}
