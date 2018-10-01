#pragma once

enum class TransformComponentType
{
	NONE = 0,
	TransformComponent,
	Transform_PhysiscComponent,
	Transfrom_FollowCamera,
};
enum class MaterialComponentType
{
	NONE = 0,
	MaterialComponent,
};
enum class InputControllerType
{
	NONE = 0,
	UserControllerComponent,
	MonsterAIComponent,
}; 
enum class CustumBufferType
{
	SkyPlane = 0,

};

class GameObject;
class ComponentFactory
{
public:
	ComponentFactory();
	~ComponentFactory();

	pair<ComponentFamilyID, IComponent*> Create(TransformComponentType type, GameObject* object);
	pair<ComponentFamilyID, IComponent*> Create(MaterialComponentType type, GameObject* object);
	pair<ComponentFamilyID, IComponent*> Create(InputControllerType type, GameObject* object);
	pair<ComponentFamilyID, IComponent*> Create(CustumBufferType type, GameObject* object);

private:
	
};