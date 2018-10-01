#pragma once

class GameObject;
class GameObjectFactory
{
public:
	GameObjectFactory();
	~GameObjectFactory();

	GameObject* Create(string objectName, GlobalValues* values);
	GameObject* CreateModel(wstring modelName, GlobalValues* values, bool isUser = false);


private:
	class ComponentFactory* componentFactory;
	class MeshFactory* meshFactory;
	class TerrainMeshFactory* terrainMeshFactory;
};