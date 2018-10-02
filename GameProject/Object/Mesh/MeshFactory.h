#pragma once
#include "MeshData.h"

struct customKey 
{
	size_t size;
	wstring path;

	customKey() { 
		size = 0; 
		path = L""; 
	}

	bool const operator==(const customKey &k) {
		return size == k.size && path == k.path;
	}
	bool operator<(const customKey &k) const {
		return size < k.size;
	}
};

class MeshFactory
{
public:
	MeshFactory();
	~MeshFactory();

	template <typename T>
	MeshData<T>* Create(wstring path, MeshType meshType = MeshType::BinaryMeshFile);
	
private:
	template <typename T>
	MeshData<T>* CreateMeshData(wstring path, MeshType meshType);

private:
	map<customKey, void*> datas; // 모든타입을 다 저장하기 위해 void* 로 받는다
};

template <typename T>
inline MeshData<T>* MeshFactory::Create(wstring path, MeshType meshType)
{
	customKey key;
	key.size = sizeof(T);
	key.path = path;

	map<customKey, void*>::iterator it;
	it = datas.find(key);

	if (it == datas.end())
	{
		return CreateMeshData<T>(path, meshType);
	}
	else
	{
		return (MeshData<T>*)(it->second);
	}
}

template<typename T>
inline MeshData<T>* MeshFactory::CreateMeshData(wstring path, MeshType meshType)
{
	customKey key;
	key.size = sizeof(T);
	key.path = path;

	MeshData<T>* temp = new MeshData<T>;
	temp->Initalize(meshType, path);
	
	datas.insert(make_pair(key, (void*)temp));

	return temp;
}