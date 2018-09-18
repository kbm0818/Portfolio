#pragma once

class Shader
{
public:
	Shader(wstring shaderFile);
	virtual ~Shader();

	virtual void Render();
	virtual void PostRender() {}

	void CreateInputLayout();
	void CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * desc, UINT count);

	virtual void SetStates();

private:
	void CreateVertexShader();
	void CreatePixelShader();
	void CheckShaderError(HRESULT hr, ID3DBlob* error);
	
protected:
	wstring shaderFile;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;

	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexBlob;
	ID3DBlob* pixelBlob;

	ID3D11ShaderReflection* reflection;
};