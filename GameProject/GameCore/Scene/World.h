#pragma once
#include <list>

class IScene;
class Camera;
class Area;
class World
{
public:
	enum ObjectType { eNone, eStatic, eAlly, eMonster, eNPC, eOther, eEnd };

	World(IScene* parent, int areaSize, int areaCount, int heightSize, int heightCount);
	~World();

	void AddObject(ObjectType type, GameObject* obj, D3DXVECTOR3 pos = D3DXVECTOR3(-1,-1,-1));
	void AddGlobalObject(GameObject* obj, D3DXVECTOR3 pos = D3DXVECTOR3(-1, -1, -1));

	void Update();
	void LateUpdate();
	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

	Camera* GetMainCamera() { return mainCamera; }
	D3DXVECTOR3 GetCenterPosition();

	void GetNearObjects(D3DXVECTOR3 position, vector<GameObject*>& v);

private:
	friend class Area;

	IScene * parentScene;
	
	Area*** areas;
	int areaSize;
	int heightSize;
	int areaCount;
	int heightCount;

	Camera* mainCamera;
	class GlobalLightBuffer* globalLight;
	class ViewProjectionBuffer* viewProjection;
	class Perspective* perspective;
	class Viewport* viewport;

	list<GameObject*> globalObjects;
};


class Area
{
public:
	Area()
	{
	}
	~Area()
	{
		for (int i = 0; i < (int)World::eEnd; ++i)
		{
			for (Pair obj : objects[i])
			{
				SAFE_DELETE(obj.second);
			}
		}
	}
	void SetWorld(World* world, D3DXVECTOR3 index, int areaSize, int heightSize)
	{
		this->world = world;
		this->areaSize = areaSize;
		this->heightSize = heightSize;
		this->index = index;
	}
	void GetObjects(vector<GameObject*>& v)
	{
		for (int i = 0; i < (int)World::eEnd; ++i)
		{
			for (Pair obj : objects[i])
			{
				if (obj.second->IsActive())
					v.push_back(obj.second);
			}
		}
	}

	void PushObject(World::ObjectType type, GameObject* obj)
	{
		D3DXVECTOR3 position = obj->GetWorldPosition();
		objects[(int)type].push_back(make_pair(position, obj));
	}

	void Update()
	{
		for (int i = 0; i < (int)World::eEnd; ++i)
		{
			for (Pair obj : objects[i])
			{
				if (obj.second->IsActive())
					obj.second->Update();
			}
		}
	}
	void LateUpdate();
	void UpdateArea();
	void PreRender()
	{
		for (int i = 0; i < (int)World::eEnd; ++i)
		{
			for (Pair obj : objects[i])
			{
				if (obj.second->IsActive())
					obj.second->PreRender();
			}
		}
	}
	void Render()
	{
		for (int i = 0; i < (int)World::eEnd; ++i)
		{
			for (Pair obj : objects[i])
			{
				if (obj.second->IsActive())
					obj.second->Render();
			}
		}
	}
	void PostRender()
	{
		for (int i = 0; i < (int)World::eEnd; ++i)
		{
			for (Pair obj : objects[i])
			{
				if (obj.second->IsActive())
				{
					obj.second->PostRender();
					ImGui::Text("%s: %d,%d,%d Area\n", obj.second->name.c_str(),
						(int)index.x, (int)index.y, (int)index.z);
				}
			}
		}
	}
private:
	typedef pair<D3DXVECTOR3, GameObject*> Pair;
	World * world;
	list<Pair> objects[(int)World::eEnd];

	int areaSize;
	int heightSize;
	D3DXVECTOR3 index;
};