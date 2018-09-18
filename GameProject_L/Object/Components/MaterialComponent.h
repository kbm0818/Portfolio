#pragma once
#include "Object/Components/IComponent.h"

class MaterialBuffer : public ShaderBuffer
{
public:
	MaterialBuffer()
	{
	}

	~MaterialBuffer()
	{
		colors.clear();
	}

	void ResetBuffer()
	{
		SAFE_RELEASE(buffer);
		data = &(colors[0]);
		dataSize = sizeof(D3DXCOLOR) * colors.size();

		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = dataSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &buffer);
		assert(SUCCEEDED(hr));
	}

	size_t GetSize()
	{
		return colors.size();
	}

	void AddColor(D3DXCOLOR color)
	{
		colors.push_back(color);
		
		ResetBuffer();
	}

	/*void SetColor(UINT index, D3DXCOLOR color)
	{
		if (index >= data.colors.size())
			return;

		data.colors[index] = color;
	}

	D3DXCOLOR GetColor(UINT index)
	{
		if (index >= data.colors.size())
			return D3DXCOLOR(0.0f,0.0f,0.0f,0.0f);

		return data.colors[index];
	}*/

private:
	vector<D3DXCOLOR> colors;
};

enum class ShaderType;
class Shader;
class MaterialComponent : public IComponent
{
public:
	MaterialComponent();
	virtual ~MaterialComponent();

	virtual void Update(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);

	void AddSRV(ID3D11ShaderResourceView* srv);
	void AddColor(D3DXCOLOR color);

	void SetShader(Shader* shader, ShaderType type);
	Shader* GetShader() { return shader; }

	void SetSamplerType(SamplerType type) { samplerType = type; }

	MaterialBuffer* GetMaterialBuffer() { return materialBuffer; }

protected:
	MaterialBuffer* materialBuffer;

	vector<ID3D11ShaderResourceView*> views;
	//0: ambient, 1: diffuse, 2:specular, 3.emissive, 4.normal
	Shader* shader;
	ShaderType shaderType;
	SamplerType samplerType;
};