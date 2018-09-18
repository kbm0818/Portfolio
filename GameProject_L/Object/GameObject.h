#pragma once

enum class ComponentFamilyID;
class IComponent;
class IMeshData;
class GameObject : public IGameObject
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Update(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);
	virtual void ResizeScreen(GlobalValues* exValue);

	IComponent* GetComponent(ComponentFamilyID id);
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