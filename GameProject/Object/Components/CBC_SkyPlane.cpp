#include "stdafx.h"
#include "CBC_SkyPlane.h"
#include "CBuffer.h"

CBC_SkyPlane::CBC_SkyPlane()
	: scale(0.3f), brightness(0.5f), translation(0.0f)
{
	cbuffer = new CBuffer<float>;

	cbuffer->AddData(scale);
	cbuffer->AddData(brightness);
	cbuffer->AddData(translation);
}

CBC_SkyPlane::~CBC_SkyPlane()
{
	SAFE_DELETE(cbuffer);
}

void CBC_SkyPlane::Update(GlobalValues * exValue)
{
	translation += 0.0001f;
	if (translation > 1.0f)
	{
		translation -= 1.0f;
	}
	cbuffer->SetData(2, translation);
}

void CBC_SkyPlane::PreRender(GlobalValues * exValue)
{
}

void CBC_SkyPlane::Render(GlobalValues * exValue)
{
	cbuffer->SetPSBuffer(1);
}

void CBC_SkyPlane::PostRender(GlobalValues * exValue)
{
}
