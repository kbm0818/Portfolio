#pragma once

class Collider
{
public:
	enum Type{ NONE, CAPSULE, END };
	enum State{ None, Enter, Stay };

	Collider(Type type);
	virtual ~Collider();

	virtual void SetPosition(D3DXVECTOR3 pos) { position = pos;  UpdateWorldPosition(); }
	virtual void SetRotation(D3DXQUATERNION rot) { rotation = rot; UpdateWorldRotation(); }

	void SetParentPosition(D3DXVECTOR3 pos) { parentPosition = pos; UpdateWorldPosition(); }
	void SetParentRotation(D3DXQUATERNION rot) { parentRotation = rot; UpdateWorldRotation(); }

	virtual D3DXVECTOR3 GetPosition() { return position; }
	virtual D3DXQUATERNION GetRotation() { return rotation; }
	D3DXVECTOR3 GetWorldPosition() { return position + parentPosition; }
	void SetParentMatrix(D3DXMATRIX& mat)
	{
		memcpy(&matParent, mat, sizeof(D3DXMATRIX));
	}

	void CheckExit();
	virtual void Render();
	virtual bool IsCollision(Collider* other) = 0;
	void Collision(Collider* other);

	void SetName(wstring name) { this->name = name; }
	void SetGameObject(GameObject* obj) { gameObject = obj; }
	wstring GetName() { return name; }
	string GetStringName();

	Type GetType() { return type; }
	string GetTypeName();

	D3DXVECTOR3 GetForward() { return forward; }
	D3DXVECTOR3 GetUp() { return up; }
	D3DXVECTOR3 GetRight() { return right; }

	D3DXVECTOR3 GetLocalForward();
	D3DXVECTOR3 GetLocalUp();
	D3DXVECTOR3 GetLocalRight();

protected:
	Type type;
	State state;

	wstring name;
	GameObject* gameObject;
	int collisionCount;

	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;

	D3DXVECTOR3 parentPosition;
	D3DXQUATERNION parentRotation;
	D3DXMATRIX matParent;

	D3DXQUATERNION prevRotation;

	D3DXVECTOR3 worldPosition;
	D3DXQUATERNION worldRotation;

	D3DXVECTOR3 forward;
	D3DXVECTOR3 up;
	D3DXVECTOR3 right;

	void UpdateAxis();
	void UpdateWorldPosition() { worldPosition = position + parentPosition; }
	void UpdateWorldRotation() { worldRotation = rotation * parentRotation; }
};