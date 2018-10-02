#pragma once 

class SRVFactory
{
public:
	SRVFactory();
	~SRVFactory();

	ID3D11ShaderResourceView* Create(wstring keyName);

private:
	map<wstring, ID3D11ShaderResourceView*> datas;
};