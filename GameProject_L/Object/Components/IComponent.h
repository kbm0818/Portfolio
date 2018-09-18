#pragma once

//FamilyID를 통해 동일 컴포넌트가 추가되지 않도록 한다
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