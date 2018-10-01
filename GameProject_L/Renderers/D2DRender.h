#pragma once

class D2DRender
{
public:
	D2DRender(GlobalValues* values);
	~D2DRender();

	void Init(ID3D11ShaderResourceView* view, D3DXVECTOR2 pos, D3DXVECTOR2 scale, Shader* shader = nullptr);

	void SetTranslation(D3DXVECTOR2& translation)
	{
		this->translation = translation;

		D3DXMatrixTranslation(&T, translation.x, translation.y, 0);

		world = S * T;
	}

	void SetScale(D3DXVECTOR2& scale)
	{
		this->scale = scale;

		D3DXMatrixScaling(&S, scale.x, scale.y, 1);
		//S = invCenter * S * center;

		world = S * T;
	}

	void Render();

private:
	class Shader* shader;
	ID3D11ShaderResourceView* srv;
	GlobalValues* values;
	class TransformC* t;

	D3DXVECTOR2 translation;
	D3DXVECTOR2 scale;
	D3DXMATRIX S, T;

	D3DXMATRIX world, view;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
};