#pragma once

class GameObject;
class ComponentBase
{
public:
	ComponentBase(bool enable = true) : 
		enable(enable), bAwaken(false), bStarted(false)
	{
		gameObject = nullptr;
	}
	virtual ~ComponentBase() 
	{ 
		gameObject->RemoveAllEvent(name);
	}

	void Enable() 
	{
		enable = true; 
		if (!bAwaken)
		{
			Awake();
			bAwaken = true;
		}
	}
	virtual void Disable() { enable = false; bStarted = false; }
	bool IsEnable() { return enable; }

	GameObject* GetGameObject() { return gameObject; }

	virtual void RegistEvent() {}

	virtual void Awake() {} //������Ʈ�� ������ Ÿ�� ���� �� 1�� ����.
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void PreRender() {}
	virtual void Render() {}
	virtual void PostRender() {}

protected:
	friend class GameObject;

	string name;
	GameObject* gameObject; //parent
	bool enable;

	bool bAwaken;
	bool bStarted;
};