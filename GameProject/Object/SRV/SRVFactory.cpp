#include "stdafx.h"
#include "SRVFactory.h"

SRVFactory::SRVFactory()
{

}

SRVFactory::~SRVFactory()
{
	for (auto it = datas.begin(); it != datas.end(); it++)
	{
		SAFE_RELEASE(it->second);
	}
	datas.clear();
}

ID3D11ShaderResourceView * SRVFactory::Create(wstring keyName)
{
	map<wstring, ID3D11ShaderResourceView*>::iterator it;
	ID3D11ShaderResourceView* temp = nullptr;

	it = datas.find(keyName);

	if (it == datas.end())
	{
		Texture::LoadTexture(keyName, &temp);
	
		datas.insert(make_pair(keyName, temp));
	}
	else
	{
		temp = it->second;
	}

	return temp;
}



