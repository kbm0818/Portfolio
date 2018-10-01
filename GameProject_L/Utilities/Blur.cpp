#include "stdafx.h"
#include "Blur.h"
#include "Systems\Renders\D2DRender.h"
#include "Object\Shader\ShaderFactory.h"

Blur::Blur(GlobalValues* values)
	: srvTarget(nullptr)
{
	renderTarget = new RenderTarget;
	d2dRender = new D2DRender(values);
	buffer = new BlurBuffer;
	blurShader = values->ShaderFactory->Create(Shaders + L"GausianBlurShader.hlsl", ShaderType::VP);
	//GausianBlurShader , LinearBlurShader
}

Blur::~Blur()
{
	SAFE_DELETE(buffer);
	SAFE_RELEASE(srvTarget);
	SAFE_DELETE(renderTarget);
	SAFE_DELETE(d2dRender);
}

ID3D11ShaderResourceView * Blur::BlurTarget(wstring filePath)
{
	if (srvTarget != nullptr)
		SAFE_RELEASE(srvTarget);

	D3DDesc desc;
	D3D::GetDesc(&desc);

	buffer->SetWidth(1.0f);
	buffer->SetHeight(0.5f);

	Texture::LoadTexture(filePath, &srvTarget);
	d2dRender->Init(srvTarget, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(desc.Width, desc.Height), blurShader);

	Render();

	return renderTarget->GetSRV();
}

ID3D11ShaderResourceView * Blur::BlurTarget(ID3D11ShaderResourceView * targetView)
{
	if (srvTarget != nullptr)
		SAFE_RELEASE(srvTarget);

	D3DDesc desc;
	D3D::GetDesc(&desc);

	buffer->SetWidth(desc.Width);
	buffer->SetHeight(desc.Height);

	srvTarget = targetView;
	d2dRender->Init(srvTarget, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(desc.Width, desc.Height), blurShader);

	Render();
	
	renderTarget->SaveTexture(Contents + L"abc.png");

	return renderTarget->GetSRV();
}

void Blur::Render()
{
	D3D::Get()->SetRenderTarget(renderTarget->GetRTV());
	D3D::Get()->Clear(renderTarget->GetRTV());
	{
		buffer->SetVSBuffer(3);
		d2dRender->Render();
	}
}
