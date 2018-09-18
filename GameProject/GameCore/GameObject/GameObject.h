#pragma once
#include <vector>
#include <unordered_map>
#include "IEventArgs.h"

class ComponentBase;
class Transform;
class GameObject
{
public:
	typedef function<void(IEventArgs& eventArgs)> EventCallback;

	GameObject();
	virtual ~GameObject();

	virtual void Update();
	virtual void LateUpdate();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

	Transform* GetTransform() { return transform; }
	void SetActive(bool set) { active = set; }
	bool IsActive() { return active && !destroy; }
	void SetDestroy() { destroy = true; }
	bool IsDestroy() { return destroy; }

	//컴포넌트 추가, 삭제
	template <typename T>
	T* GetComponent();
	template <typename T>
	void GetComponentAllChilds(vector<T*>& vOut);
	template <typename T>
	T* AddComponent();
	template <typename T>
	void RemoveComponent();
	void RemoveComponent(ComponentBase* component);

	//Event 추가, 삭제
	void AddEvent(string eventName, string registrant, EventCallback callback);
	void RemoveAllEvent(string registrant);
	void SendEvent(string eventName, IEventArgs& eventArgs);

	string name;

protected:
	bool active;
	Transform * transform;

	//중요한 대원칙: 같은 컴포넌트를 중복해서 추가하지 않는다. 
	//필요한 경우 ColliderManager처럼 묶어서 관리하는 컴포넌트를 만든다.
	vector<ComponentBase*> components;

private:
	typedef vector<pair<string,EventCallback>> Events;

	bool destroy;
	unordered_map<string, Events> events;

public:
	//Wrapped Method
	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXQUATERNION rot);
	void SetScale(D3DXVECTOR3 s);

	D3DXVECTOR3 GetPosition();
	D3DXQUATERNION GetRotation();
	D3DXVECTOR3 GetScale();

	D3DXVECTOR3 Forward();
	D3DXVECTOR3 Up();
	D3DXVECTOR3 Right();

	D3DXVECTOR3 GetWorldPosition();
	D3DXQUATERNION GetWorldRotation();
	D3DXVECTOR3 GetWorldScale();
};

//컴포넌트 검색
template <typename T>
T* GameObject::GetComponent()
{
	string key = typeid(T).name();

	size_t end = components.size();
	for (size_t i = 0; i < end; ++i)
	{
		if (components[i]->name == key)
		{
			return (T*)(components[i]);
		}
	}
	return nullptr;
}

template <typename T>
void GameObject::GetComponentAllChilds(vector<T*>& vOut)
{
	T* component = this->GetComponent<T>();
	if (component)
		vOut.push_back(component);

	for (Transform* t : transform->childs)
	{
		t->gameObject->GetComponentAllChilds<T>(vOut);
	}
	
}

template <typename T>
T* GameObject::AddComponent()
{
	T* component = new T;

	components.push_back(component);
	component->gameObject = this;
	component->name = typeid(T).name();
	return component;
}

template <typename T>
void GameObject::RemoveComponent()
{
	string key = typeid(T).name();
	size_t end = components.size();
	//0번 컴포넌트는 transform인데, 삭제될 수 없으므로 1번부터 검색을 시작한다.
	//컴포넌트 추가 시 insert는 지원하지 말 것.
	for (size_t i = 1; i < end; ++i)
	{
		if (components[i]->name == key)
		{
			SAFE_DELETE(components[i]);
			components.erase(components.begin() + i);
			break;
		}
	}
}
