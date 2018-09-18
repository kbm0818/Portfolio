#include "stdafx.h"
#include "HitCheck.h"
#include "GameCore\Resource\Material.h"

void HitCheck::Awake()
{
	gameObject->GetComponentAllChilds<Renderer>(renderers);
}

void HitCheck::RegistEvent()
{
	gameObject->AddEvent("OnCollisionEnter", name, [this](IEventArgs& e)
	{
		this->OnCollisionEnter((CollisionEventArgs&)e);
	});
	//gameObject->AddEvent("OnCollisionEnter", name, [this](IEventArgs& e)
	//{
	//	this->OnCollisionExit((CollisionEventArgs&)e);
	//});
}

void HitCheck::Update()
{
	timer -= Time::Delta();
	if (timer <= 0)
	{
		OnCollisionExit(CollisionEventArgs(nullptr));
		timer = 999999;
	}
}

void HitCheck::OnCollisionEnter(CollisionEventArgs & e)
{
	for (Renderer* renderer : renderers)
	{
		renderer->GetMaterial()->SetDiffuse(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
	timer = 0.5f;
}

void HitCheck::OnCollisionExit(CollisionEventArgs & e)
{
	for (Renderer* renderer : renderers)
	{
		renderer->GetMaterial()->SetDiffuse(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}
