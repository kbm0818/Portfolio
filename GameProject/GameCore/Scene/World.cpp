#include "stdafx.h"
#include "World.h"
#include "IScene.h"
#include ".\Viewer\FirstPerson.h"
#include "./Viewer/Perspective.h"
#include "./Viewer/Viewport.h"

World::World(IScene* parent, int areaSize, int areaCount, int heightSize, int heightCount)
	: parentScene(parent), areaSize(areaSize), heightSize(heightSize),
	areaCount(areaCount), heightCount(heightCount)
{
	//y, x, z
	areas = new Area**[heightCount];
	for (int i = 0; i < heightCount; ++i)
	{
		areas[i] = new Area*[areaCount];
		for (int j = 0; j < areaCount; ++j)
		{
			areas[i][j] = new Area[areaCount];
			for (int z = 0; z < areaCount; ++z)
				areas[i][j][z].SetWorld(this,
					D3DXVECTOR3((float)j, (float)i, (float)z), 
					areaSize,heightSize);
		}
	}

	D3DDesc desc;
	D3D::GetDesc(&desc);

	globalLight = new GlobalLightBuffer();
	viewProjection = new ViewProjectionBuffer();
	perspective = new Perspective(desc.Width, desc.Height);
	viewport = new Viewport(desc.Width, desc.Height);
	mainCamera = new FirstPerson();

	mainCamera->SetPosition(0, 1, -3);

}

World::~World()
{
	SAFE_DELETE(globalLight);
	SAFE_DELETE(viewProjection);
	SAFE_DELETE(perspective);
	SAFE_DELETE(mainCamera);
	SAFE_DELETE(viewport);

	for (int i = 0; i < heightCount; ++i)
	{
		for (int j = 0; j < areaCount; ++j)
			delete[] areas[i][j];
	}
	for (int i = 0; i < heightCount; ++i)
		delete[] areas[i];

	delete[] areas;

	for (GameObject* obj : globalObjects)
		delete obj;
}

void World::AddObject(ObjectType type, GameObject * obj, D3DXVECTOR3 pos)
{
	D3DXVECTOR3 objectPosition = obj->GetWorldPosition();
	if (pos == D3DXVECTOR3(-1, -1, -1))
		obj->SetPosition(this->GetCenterPosition());

	int x = (int)objectPosition.x / areaSize;
	int y = (int)objectPosition.y / heightSize;
	int z = (int)objectPosition.z / areaSize;

	areas[y][x][z].PushObject(type, obj);
}

void World::AddGlobalObject(GameObject * obj, D3DXVECTOR3 pos)
{
	if (pos == D3DXVECTOR3(-1, -1, -1))
		obj->SetPosition(this->GetCenterPosition());

	globalObjects.push_back(obj);
}

void World::Update()
{
	for (GameObject* obj : globalObjects)
	{
		if (obj->IsActive())
			obj->Update();
	}
	for (int height = 0; height < heightCount; ++height)
	{
		for (int x = 0; x < areaCount; ++x)
		{
			for (int z = 0; z < areaCount; ++z)
			{
				areas[height][x][z].Update();
			}
		}
	}
}

void World::LateUpdate()
{
	auto iter = globalObjects.begin();
	auto end = globalObjects.end();
	while (iter != end)
	{
		if ((*iter)->IsActive())
		{
			(*iter)->LateUpdate();
			++iter;
		}
		else if ((*iter)->IsDestroy())
		{
			SAFE_DELETE(*iter);
			iter = globalObjects.erase(iter);
			end = globalObjects.end();
		}
	}

	/*
	각종 이동처리는 반드시 Update에서 진행한다. (GlobalObjects는 상관없음.)
	LateUpdate에서 컬라이더 충돌처리 등을 진행하기 전에
	현재 최종 위치에 맞게 Area를 업데이트하는 작업이 선행된다.
	*/
	for (int height = 0; height < heightCount; ++height)
	{
		for (int x = 0; x < areaCount; ++x)
		{
			for (int z = 0; z < areaCount; ++z)
			{
				areas[height][x][z].UpdateArea();
			}
		}
	}
	for (int height = 0; height < heightCount; ++height)
	{
		for (int x = 0; x < areaCount; ++x)
		{
			for (int z = 0; z < areaCount; ++z)
			{
				areas[height][x][z].LateUpdate();
			}
		}
	}
}

void World::PreRender()
{
	mainCamera->Update();

	for (GameObject* obj : globalObjects)
	{
		if (obj->IsActive())
			obj->PreRender();
	}

	for (int height = 0; height < heightCount; ++height)
	{
		for (int x = 0; x < areaCount; ++x)
		{
			for (int z = 0; z < areaCount; ++z)
			{
				areas[height][x][z].PreRender();
			}
		}
	}
}

void World::Render()
{
	D3DXMATRIX view, projection;
	mainCamera->GetMatrix(&view);
	perspective->GetMatrix(&projection);

	viewProjection->SetView(view);
	viewProjection->SetProjection(projection);
	viewProjection->SetVSBuffer(0);

	globalLight->SetPSBuffer(0);

	for (GameObject* obj : globalObjects)
	{
		if (obj->IsActive())
			obj->Render();
	}

	for (int height = 0; height < heightCount; ++height)
	{
		for (int x = 0; x < areaCount; ++x)
		{
			for (int z = 0; z < areaCount; ++z)
			{
				areas[height][x][z].Render();
			}
		}
	}
}

void World::PostRender()
{
	for (GameObject* obj : globalObjects)
	{
		if (obj->IsActive())
			obj->PostRender();
	}

	for (int height = 0; height < heightCount; ++height)
	{
		for (int x = 0; x < areaCount; ++x)
		{
			for (int z = 0; z < areaCount; ++z)
			{
				areas[height][x][z].PostRender();
			}
		}
	}
}

void World::ResizeScreen()
{
	D3DXVECTOR2 screen = SceneManager::Get()->GetScreenSize();
	perspective->Set(screen.x, screen.y);
	viewport->Set(screen.x, screen.y);
}

D3DXVECTOR3 World::GetCenterPosition()
{
	D3DXVECTOR3 center;
	center.x = areaSize * areaCount * 0.5f;
	center.z = center.x;
	center.y = 0;
	return center;
}

void World::GetNearObjects(D3DXVECTOR3 position, vector<GameObject*>& v)
{
	D3DXVECTOR3 index;
	index.x = position.x / (float)areaSize;
	index.y = position.y / (float)heightSize;
	index.z = position.z / (float)areaSize;

	int x = (int)index.x;
	int y = (int)index.y;
	int z = (int)index.z;
	areas[y][x][z].GetObjects(v);

	//인접한 지역 체크
	if (index.x - (float)x >= 0.75f)
	{
		if (x + 1 < areaCount)
			areas[y][x + 1][z].GetObjects(v);
	}
	else if (index.x - (float)x <= 0.25f)
	{
		if (x - 1 > 0)
			areas[y][x - 1][z].GetObjects(v);
	}

	if (index.y - (float)y >= 0.75f)
	{
		if (y + 1 < heightCount)
			areas[y+1][x][z].GetObjects(v);
	}
	else if (index.y - (float)y <= 0.25f)
	{
		if (y - 1 > 0)
			areas[y-1][x][z].GetObjects(v);
	}

	if (index.z - (float)z >= 0.75f)
	{
		if (z + 1 < areaCount)
			areas[y][x][z+1].GetObjects(v);
	}
	else if (index.z - (float)z <= 0.25f)
	{
		if (z - 1 > 0)
			areas[y][x][z-1].GetObjects(v);
	}
}









void Area::LateUpdate()
{
	for (int i = 0; i < (int)World::eEnd; ++i)
	{
		auto iter = objects[i].begin();
		auto end = objects[i].end();
		while (iter != end)
		{
			if ((*iter).second->IsActive())
			{
				(*iter).second->LateUpdate();
				++iter;			
			}
			//만약 오브젝트가 파괴되었을 경우 처리.
			//다른 GameObject 포인터를 가지고 작업하는 스크립트는
			//Update 끝쪽에 반드시 해당 GameObject의 destroy 여부를 
			//체크하여 nullptr로 초기화할 것.
			else if((*iter).second->IsDestroy())
			{
				SAFE_DELETE((*iter).second);
				iter = objects[i].erase(iter);
				end = objects[i].end();
			}
			else ++iter;
		}
	}
}

void Area::UpdateArea()
{
	for (int i = 0; i < (int)World::eEnd; ++i)
	{
		auto iter = objects[i].begin();
		auto end = objects[i].end();
		while (iter != end)
		{
			//위치 이동 시 지역 재설정 체크.
			D3DXVECTOR3 newPosition = (*iter).second->GetWorldPosition();
			if ((*iter).first != newPosition)
			{
				D3DXVECTOR3 newIndex;
				newIndex.x = (float)((int)(newPosition.x / (float)areaSize));
				newIndex.y = (float)((int)(newPosition.y / (float)heightSize));
				newIndex.z = (float)((int)(newPosition.z / (float)areaSize));
				if (index != newIndex)
				{
					world->areas[(int)newIndex.y][(int)newIndex.x][(int)newIndex.z].PushObject(
						World::ObjectType(i), (*iter).second);

					iter = objects[i].erase(iter);
					end = objects[i].end();
				}
				else ++iter;
			}
			else ++iter;
		}
	}
}


