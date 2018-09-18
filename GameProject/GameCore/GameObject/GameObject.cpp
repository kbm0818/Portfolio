#include "stdafx.h"
#include "GameObject.h"
#include "./Components/Transform.h"
#include "./Components/ComponentBase.h"

GameObject::GameObject()
{
	transform = AddComponent<Transform>();
	destroy = false;
}

GameObject::~GameObject()
{
	for (ComponentBase* component : components)
	{
		SAFE_DELETE(component);
	}
	components.clear();
	events.clear();
}

void GameObject::Update()
{
	for (ComponentBase* component : components)
	{
		if (!component->bAwaken)
		{
			component->Awake();
			component->RegistEvent();
			component->bAwaken = true;
		}
		if (!component->bStarted)
		{
			component->Start();
			component->bStarted = true;
		}
		if (component->enable)
			component->Update();
	}
}

void GameObject::LateUpdate()
{
	for (ComponentBase* component : components)
	{
		if (component->enable)
			component->LateUpdate();
	}
}

void GameObject::PreRender()
{
	for (ComponentBase* component : components)
	{
		if (component->enable)
			component->PreRender();
	}
}

void GameObject::Render()
{
	for (ComponentBase* component : components)
	{
		if (component->enable)
			component->Render();
	}
}

void GameObject::PostRender()
{
	for (ComponentBase* component : components)
	{
		if (component->enable)
			component->PostRender();
	}
}

void GameObject::RemoveComponent(ComponentBase * component)
{
	size_t end = components.size();
	for (size_t i = 1; i < end; ++i)
	{
		if (components[i] == component)
		{
			SAFE_DELETE(components[i]);
			components.erase(components.begin() + i);
			return;
		}
	}
}

void GameObject::AddEvent(string eventName, string registrant, EventCallback callback)
{
	events[eventName].push_back(make_pair(registrant, callback));
}

void GameObject::RemoveAllEvent(string registrant)
{
	for (auto iter = events.begin(); iter != events.end(); ++iter)
	{
		auto& callbacks = (*iter).second;
		for (UINT i = 0; i < callbacks.size(); ++i)
		{
			if (callbacks[i].first == registrant)
			{
				callbacks.erase(callbacks.begin() + i);
				--i;
			}
		}
	}
}

void GameObject::SendEvent(string eventName, IEventArgs & eventArgs)
{
	auto& callbacks = events[eventName];
	for (UINT i = 0; i < callbacks.size(); ++i)
	{
		callbacks[i].second(eventArgs);
	}
}

void GameObject::SetPosition(D3DXVECTOR3 pos)
{
	transform->SetPosition(pos);
}

void GameObject::SetRotation(D3DXQUATERNION rot)
{
	transform->SetRotation(rot);
}

void GameObject::SetScale(D3DXVECTOR3 s)
{
	transform->SetScale(s);
}

D3DXVECTOR3 GameObject::GetPosition()
{
	return transform->GetPosition();
}

D3DXQUATERNION GameObject::GetRotation()
{
	return transform->GetRotation();
}

D3DXVECTOR3 GameObject::GetScale()
{
	return transform->GetScale();
}

D3DXVECTOR3 GameObject::Forward()
{
	return transform->forward;
}

D3DXVECTOR3 GameObject::Up()
{
	return transform->up;
}

D3DXVECTOR3 GameObject::Right()
{
	return transform->right;
}

D3DXVECTOR3 GameObject::GetWorldPosition()
{
	return transform->GetWorldPosition();
}

D3DXQUATERNION GameObject::GetWorldRotation()
{
	return transform->GetWorldRotation();
}

D3DXVECTOR3 GameObject::GetWorldScale()
{
	return transform->GetWorldScale();
}

