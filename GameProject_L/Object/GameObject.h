#pragma once

enum class ComponentFamilyID;
class IComponent;
class IMeshData;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void Update(GlobalValues* values);
	void PreRender(GlobalValues* values);
	void Render(GlobalValues* values);
	void PostRender(GlobalValues* values);
	void ResizeScreen(GlobalValues* values);
	void DepthRender(GlobalValues* values);

	template <typename T>
	T* GetMesh();

	template <typename T2>
	T2* GetComponent(ComponentFamilyID id);

	void DeleteComponent(ComponentFamilyID id);

	void SetComponent(ComponentFamilyID id, IComponent* component);
	void SetComponent(pair<ComponentFamilyID, IComponent*> componentPair);

	void SetMesh(IMeshData* mesh);



private:
	friend class GameObjectFactory;
	
	map<ComponentFamilyID, IComponent*> components;
	map<ComponentFamilyID, IComponent*>::iterator iter;

	IMeshData* mesh;
};

template<typename T>
inline T * GameObject::GetComponent(ComponentFamilyID id)
{
	iter = components.find(id);
	return (iter == components.end() ? nullptr : dynamic_cast<T*>(iter->second));
}

template<typename T>
inline T * GameObject::GetMesh()
{
	return dynamic_cast<T*>(mesh);
}
