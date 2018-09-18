#include "stdafx.h"
#include "Material.h"




Material::Material()
{
	ambient = D3DXCOLOR(0, 0, 0, 1);
	emissive = D3DXCOLOR(0, 0, 0, 1);
	diffuse = D3DXCOLOR(0, 0, 0, 1);
	specular = D3DXCOLOR(0, 0, 0, 1);
	shininess = 0;

	ambientView = NULL;
	diffuseView = NULL;
	specularView = NULL;
	emissiveView = NULL;

	materialBuffer = new MaterialBuffer;
}

Material::~Material()
{
	SAFE_DELETE(materialBuffer);

	SAFE_RELEASE(ambientView);
	SAFE_RELEASE(emissiveView);
	SAFE_RELEASE(diffuseView);
	SAFE_RELEASE(specularView);
}

void Material::SetAmbient(D3DXCOLOR & color)
{
	ambient = color;
}

void Material::SetDiffuse(D3DXCOLOR & color)
{
	diffuse = color;
}

void Material::SetSpecular(D3DXCOLOR & color)
{
	specular = color;
}

void Material::SetEmissive(D3DXCOLOR & color)
{
	emissive = color;
}

void Material::SetAmbientTexture(wstring file)
{
	SAFE_RELEASE(ambientView);
	CreateView(AMBIENT, file, &ambientView);
}

void Material::SetDiffuseTexture(wstring file)
{
	SAFE_RELEASE(diffuseView);
	CreateView(DIFFUSE, file, &diffuseView);
}

void Material::SetSpecularTexture(wstring file)
{
	SAFE_RELEASE(specularView);
	CreateView(SPECULAR, file, &specularView);
}

void Material::SetEmissiveTexture(wstring file)
{
	SAFE_RELEASE(emissiveView);
	CreateView(EMISSIVE, file, &emissiveView);
}

void Material::SetPsBuffer(int slot)
{
	materialBuffer->SetAmbient(ambient);
	materialBuffer->SetDiffuse(diffuse);
	materialBuffer->SetEmissive(emissive);
	materialBuffer->SetSpecular(specular);
	materialBuffer->SetShininess(shininess);

	materialBuffer->SetPSBuffer(slot);
}

void Material::CreateView(MaterialType type, wstring path, ID3D11ShaderResourceView** view)
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
}