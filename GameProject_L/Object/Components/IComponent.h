#pragma once

//FamilyID�� ���� ���� ������Ʈ�� �߰����� �ʵ��� �Ѵ�
enum class ComponentFamilyID
{
	InputComponent = 0,
	TransformComponent,
	MaterialComponent,
	CustumBufferComponent,
};

struct GlobalValues;
class IComponent
{
public:
	virtual ~IComponent() {};

	virtual void Update(GlobalValues* exValue) = 0;
	virtual void Render(GlobalValues* exValue) = 0;
	virtual void PreRender(GlobalValues* exValue) = 0;
	virtual void PostRender(GlobalValues* exValue) = 0;
};