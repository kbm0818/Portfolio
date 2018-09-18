#pragma once

class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();

	ID3D11RenderTargetView* GetRTV()
	{
		return rtv;
	}

	ID3D11ShaderResourceView* GetSRV()
	{
		return srv;
	}

	void SaveTexture(wstring file);

private:
	ID3D11Texture2D* texture;

	ID3D11RenderTargetView* rtv;

	ID3D11ShaderResourceView* srv;
};