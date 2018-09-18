#pragma once
#include "ComponentBase.h"
#include <vector>

class Transform : public ComponentBase
{
public:
	Transform();
	virtual ~Transform();

	virtual void Awake();
	virtual void Update();
	virtual void PreRender();
	virtual void Render();
	virtual void PostRender();

	virtual void Disable() {}

	void SetPosition(D3DXVECTOR3 pos);
	void SetRotation(D3DXQUATERNION rot);
	void SetScale(D3DXVECTOR3 s);
	D3DXVECTOR3 GetPosition() { return position; }
	D3DXQUATERNION GetRotation() { return rotation; }
	D3DXVECTOR3 GetScale() { return scale; }

	D3DXVECTOR3 GetWorldPosition();
	D3DXQUATERNION GetWorldRotation();
	D3DXVECTOR3 GetWorldScale();

	D3DXMATRIX& GetMatrix() { return matLocal; }
	D3DXMATRIX& GetWorldMatrix();

	void AddChild(Transform* t) { childs.push_back(t); t->parent = this; }
	void RemoveChild(Transform* t);

private:
	friend class GameObject;

	void UpdateMatrix();
	void UpdateAxis();

	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;
	D3DXVECTOR3 scale;

	D3DXVECTOR3 up;
	D3DXVECTOR3 right;
	//부모 좌표계 포함 최종적으로 바라보고있는 축
	D3DXVECTOR3 forward;

	D3DXMATRIX matLocal;
	D3DXMATRIX matWorld;

	Transform* parent;
	vector<Transform*> childs;
};