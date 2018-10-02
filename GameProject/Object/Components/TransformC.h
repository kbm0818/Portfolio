#pragma once
#include "IComponent.h"

class GameObject;
class TransformC : public IComponent
{
public:
	TransformC();
	virtual ~TransformC();

	virtual void Update(GlobalValues* exValue);
	virtual void PreRender(GlobalValues* exValue);
	virtual void Render(GlobalValues* exValue);
	virtual void PostRender(GlobalValues* exValue);

	void SetPosition(const D3DXVECTOR3& vec);
	void SetScale(const D3DXVECTOR3& vec);
	void SetScale(const float& s);
	void SetRotation(const D3DXVECTOR3& vec);

	WorldBuffer* GetWorldBuffer() { return buffer; }

	void GetPosition(D3DXVECTOR3& pos) { pos = position; }
	void GetRotation(D3DXVECTOR3& rot) { rot = rotation; }
	void GetScale(D3DXVECTOR3& sc) { sc = scale; }

	void SetSlot(UINT slot) { this->slot = slot; }

	D3DXMATRIX GetWorld() { return world; }
	void SetWorld(D3DXMATRIX world) { this->world = world; }

	void CalcS();
	void CalcR();
	void CalcT();

protected:
	void UpdateWorld(GlobalValues* exValue);
	void SetWorld(GlobalValues* exValue);

	bool isMove;

	D3DXVECTOR3 position;

	D3DXVECTOR3 rotation;
	D3DXQUATERNION rotate;

	D3DXVECTOR3 scale;

	D3DXVECTOR3 center; // 스케일링과 로테이션에 중심점이동이 필요할때

	D3DXMATRIX S;
	D3DXMATRIX R;
	D3DXMATRIX T;

	D3DXMATRIX world;

	WorldBuffer* buffer;

	UINT slot;
};