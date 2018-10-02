#pragma once

class Texture
{
public:
	static void LoadTexture(wstring fileName, ID3D11Texture2D** texture);
	static void LoadTexture(string fileName, ID3D11Texture2D** texture);
	static bool LoadTexture(wstring fileName, ID3D11ShaderResourceView** view);

	static void LoadPixel(ID3D11Texture2D* texture, UINT** colors);
};