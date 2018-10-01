#include "stdafx.h"
#include "GameObject.h"

GameObject::GameObject()
	: mesh(nullptr)
{
}

GameObject::~GameObject()
{
	for (iter = components.begin(); iter != components.end(); ++iter)
		SAFE_DELETE(iter->second);

	components.clear();
}

void GameObject::Update(GlobalValues * exValue)
{
	for (iter = components.begin(); iter != components.end(); ++iter)
	{
		if(iter->second)
			iter->second->Update(exValue);
	}

	if (mesh)
		mesh->Update(exValue);
}

void GameObject::PreRender(GlobalValues * exValue)
{
	for (iter = components.begin(); iter != components.end(); ++iter)
	{
		if (iter->second)
			iter->second->PreRender(exValue);
	}
}

void GameObject::Render(GlobalValues * exValue)
{
	for (iter = components.begin(); iter != components.end(); ++iter)
	{
		if (iter->second)
			iter->second->Render(exValue);
	}

	if(mesh)
		mesh->Render(exValue);
}

void GameObject::PostRender(GlobalValues * exValue)
{
	for (iter = components.begin(); iter != components.end(); ++iter)
	{
		if (iter->second)
			iter->second->PostRender(exValue);
	}
}

void GameObject::ResizeScreen(GlobalValues * exValue)
{
}

void GameObject::DepthRender(GlobalValues * values)
{
	for (iter = components.begin(); iter != components.end(); ++iter)
	{
		if (iter->second)
			iter->second->DepthRender(values);
	}

	if (mesh)
		mesh->DepthRender(values);
}

void GameObject::DeleteComponent(ComponentFamilyID id)
{
	iter = components.find(id);
	if (iter == components.end())
		return;

	SAFE_DELETE(iter->second);
	components.erase(iter);
}

void GameObject::SetComponent(ComponentFamilyID id, IComponent * component)
{
	DeleteComponent(id);

	components.insert(make_pair(id, component));
}

void GameObject::SetComponent(pair<ComponentFamilyID, IComponent*> componentPair)
{
	DeleteComponent(componentPair.first);

	components.insert(componentPair);
}

void GameObject::SetMesh(IMeshData * mesh)
{
	this->mesh = mesh;
}

