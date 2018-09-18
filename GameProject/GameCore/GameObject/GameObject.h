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

	//������Ʈ �߰�, ����
	template <typename T>
	T* GetComponent();
	template <typename T>
	void GetComponentAllChilds(vector<T*>& vOut);
	template <typename T>
	T* AddComponent();
	template <typename T>
	void RemoveComponent();
	void RemoveComponent(ComponentBase* component);

	//Event �߰�, ����
	void AddEvent(string eventName, string registrant, EventCallback callback);
	void RemoveAllEvent(string registrant);
	void SendEvent(string eventName, IEventArgs& eventArgs);

	string name;

protected:
	bool active;
	Transform * transform;

	//�߿��� ���Ģ: ���� ������Ʈ�� �ߺ��ؼ� �߰����� �ʴ´�. 
	//�ʿ��� ��� ColliderManageró�� ��� �����ϴ� ������Ʈ�� �����.
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

//������Ʈ �˻�
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
	//0�� ������Ʈ�� transform�ε�, ������ �� �����Ƿ� 1������ �˻��� �����Ѵ�.
	//������Ʈ �߰� �� insert�� �������� �� ��.
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
