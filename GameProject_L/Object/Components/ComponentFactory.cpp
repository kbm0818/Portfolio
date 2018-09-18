#include "stdafx.h"
#include "ComponentFactory.h"

#include "../GameObject.h"

#include "TransformC.h"
#include "Transform_PhysiscC.h"
#include "Transform_FollowCamera.h"

#include "MaterialComponent.h"

#include "UserControllerC.h"

#include "CBC_SkyPlane.h"

ComponentFactory::ComponentFactory()
{
}

ComponentFactory::~ComponentFactory()
{
}

pair<ComponentFamilyID, IComponent*> ComponentFactory::Create(TransformComponentType type, GameObject* object)
{
	object->DeleteComponent(ComponentFamilyID::TransformComponent);

	switch (type)
	{
		case TransformComponentType::NONE:
			return make_pair(ComponentFamilyID::TransformComponent, nullptr);
			break;

		case TransformComponentType::TransformComponent:
			return make_pair(ComponentFamilyID::TransformComponent, new TransformC);
			break;

		case TransformComponentType::Transform_PhysiscComponent:
			return make_pair(ComponentFamilyID::TransformComponent, new Transform_PhysiscC(object));
			break;
		case TransformComponentType::Transfrom_FollowCamera:
			return make_pair(ComponentFamilyID::TransformComponent, new Transform_FollowCamera);
			break;
		default:
			return make_pair(ComponentFamilyID::TransformComponent, nullptr);
			break;
	}
}

pair<ComponentFamilyID, IComponent*> ComponentFactory::Create(MaterialComponentType type, GameObject* object)
{
	object->DeleteComponent(ComponentFamilyID::MaterialComponent);

	switch (type)
	{
	case MaterialComponentType::NONE:
		return make_pair(ComponentFamilyID::MaterialComponent, nullptr);
		break;

	case MaterialComponentType::MaterialComponent:
		return make_pair(ComponentFamilyID::MaterialComponent, new MaterialComponent);
		break;
	default:
		return make_pair(ComponentFamilyID::MaterialComponent, nullptr);
		break;
	}
}

pair<ComponentFamilyID, IComponent*> ComponentFactory::Create(InputControllerType type, GameObject* object)
{
	object->DeleteComponent(ComponentFamilyID::InputComponent);

	switch (type)
	{
	case InputControllerType::NONE:
		return make_pair(ComponentFamilyID::InputComponent, nullptr);
		break;

	case InputControllerType::UserControllerComponent:
		return make_pair(ComponentFamilyID::InputComponent, new UserControllerC(object));
		break;
	default:
		return make_pair(ComponentFamilyID::InputComponent, nullptr);
		break;
	}
}

pair<ComponentFamilyID, IComponent*> ComponentFactory::Create(CustumBufferType type, GameObject* object)
{
	object->DeleteComponent(ComponentFamilyID::CustumBufferComponent);

	switch (type)
	{
	case CustumBufferType::SkyPlane:
		return make_pair(ComponentFamilyID::CustumBufferComponent, new CBC_SkyPlane);
		break;

	default:
		return make_pair(ComponentFamilyID::CustumBufferComponent, nullptr);
		break;
	}
}
