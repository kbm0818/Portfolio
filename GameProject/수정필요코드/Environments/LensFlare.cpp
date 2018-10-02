#include "stdafx.h"
#include "LensFlare.h"
#include "../Utilities/Mesh.h"

LensFlare::LensFlare()
	: vertexBuffer(nullptr), indexBuffer(nullptr)
	, lensFlareTexture(nullptr), isVisible(false)
{
	Initialize(Textures + L"lens_flare.dds");

	lensFlareBuffer = new LensFlareBuffer();
	shader = new Shader(Shaders + L"LensFlare.hlsl", ShaderType::VP);

	// create the flare elements
	// the first is hard-coded
	m_flareElements[0].linePos = 1.0f;
	m_flareElements[0].scale = 0.25f;
	m_flareElements[0].brightness = 1.0f;
	m_flareElements[0].texU = 0.0f;
	m_flareElements[0].texV = 0.0f;

	float flareStep = 2.0f / k_totalFlares;
	float flarePos = 1.0f;

	int xPos = 0;
	int yPos = 0;

	// the rest are random
	for (int i = 1; i<k_totalFlares; ++i)
	{
		flarePos -= flareStep;
		float posJitter =
			Warehouse::RandomNumberInRange(-0.2f, 0.2f);

		m_flareElements[i].linePos =
			flarePos + posJitter;

		Warehouse::clamp(
			m_flareElements[i].linePos,
			-1.0f, 1.0f);

		m_flareElements[i].scale =
			Warehouse::RandomNumberInRange(0.3f, 2.5f);

		m_flareElements[i].scale *=
			abs(m_flareElements[i].linePos);

		m_flareElements[i].brightness =
			Warehouse::RandomNumberInRange(0.75f, 2.0f);

		++xPos;
		if (xPos>2)
		{
			xPos = 0;
			++yPos;
		}
		if (yPos>2)
		{
			yPos = 0;
		}

		m_flareElements[i].texU = 0.33f*xPos;
		m_flareElements[i].texV = 0.33f*yPos;
	}

	// okay, I lied. We force the last one to the
	// to the position opposite the sun
	m_flareElements[k_totalFlares - 1].linePos = -1.0f;

}

LensFlare::~LensFlare()
{
	SAFE_RELEASE(lensFlareTexture);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(sampleState);
	SAFE_RELEASE(alphaBlendEnableState); 
	SAFE_RELEASE(alphaBlendDisableState);

	SAFE_DELETE(lensFlareBuffer);
	SAFE_DELETE(shader);
}

void LensFlare::Update(D3DXVECTOR3 camPos, D3DXVECTOR3 sunDir, D3DXMATRIX view, D3DXMATRIX projection)
{
	D3DXVECTOR3 sunPos = camPos - sunDir;
	D3DXMATRIX mat = projection * view;
	
	D3DXVec3TransformCoord(&screenPos, &sunPos, &mat);

	// is the sun in view?
	if (screenPos.x > -1.2f
		&& screenPos.x < 1.2f
		&& screenPos.y > -1.2f
		&& screenPos.y < 1.2f
		&& screenPos.z > 0.0f)
	{
		isVisible = true;
	}
	else
	{
		isVisible = false;
	}

	if (isVisible)
	{
		D3DXVECTOR2 temp;
		temp.x = screenPos.x;
		temp.y = screenPos.y;

		lensFlareBuffer->Data.flareColor = D3DXCOLOR(1.0f, 0.8f, 0.8f, 1.0f);
		lensFlareBuffer->Data.flareColor.a = Warehouse::clamp(sunDir.z * 2.0f - 0.15f, 0.0f, 1.0f) * Warehouse::clamp(1.0f - D3DXVec2Length(&temp), 0.0f, 1.0f);
	}
	
}

void LensFlare::Render()
{
	if (true)//isVisible)
	{
		unsigned int stride;
		unsigned int offset;

		stride = sizeof(VertexType);
		offset = 0;

		D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		shader->Render(ShaderType::VP);

		D3D::GetDC()->PSSetShaderResources(0, 1, &lensFlareTexture);

		D3D::GetDC()->PSSetSamplers(0, 1, &sampleState);

		float blendFactor[4];

		// 블렌드 인수를 설정합니다.
		blendFactor[0] = 0.0f;
		blendFactor[1] = 0.0f;
		blendFactor[2] = 0.0f;
		blendFactor[3] = 0.0f;

		D3D::GetDC()->OMSetBlendState(alphaBlendEnableState, blendFactor, 0xffffffff);
		States::SetDepthStencil(depthOffState);
		for(int i = 0 ; i < k_totalFlares; i++)
		{
			lensFlareBuffer->Data.posOffSet.x = screenPos.x * m_flareElements[i].linePos;
			lensFlareBuffer->Data.posOffSet.y = screenPos.y * m_flareElements[i].linePos;
			lensFlareBuffer->Data.scale = m_flareElements[i].scale;
			lensFlareBuffer->Data.uvOffSet = D3DXVECTOR2(m_flareElements[i].texU, m_flareElements[i].texV);

			lensFlareBuffer->SetVSBuffer(3);

			D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
		}
		States::SetDepthStencil(depthOnState);
		D3D::GetDC()->OMSetBlendState(alphaBlendDisableState, blendFactor, 0xffffffff);
	}
}

void LensFlare::PostRender(GuiSettings * settings)
{
}

bool LensFlare::Initialize(wstring textureName)
{
	InitializeBuffers();
	CreateState();
	LoadTextures(textureName);

	return true;
}

bool LensFlare::InitializeBuffers()
{
	vertexCount = 4;
	VertexType vertex[4];
	vertex[0].position = D3DXVECTOR2(-1.0f, -1.0f);
	vertex[0].uv = D3DXVECTOR2(0.0f, 0.0f);
	vertex[1].position = D3DXVECTOR2(-1.0f, 1.0f);
	vertex[1].uv = D3DXVECTOR2(0.0f, 0.33f);
	vertex[2].position = D3DXVECTOR2(1.0f, -1.0f);
	vertex[2].uv = D3DXVECTOR2(0.33f, 0.0f);
	vertex[3].position = D3DXVECTOR2(1.0f, 1.0f);
	vertex[3].uv = D3DXVECTOR2(0.33f, 0.33f);

	indexCount = 6;
	UINT index[6] = { 0, 1, 2, 2, 1, 3 };

	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(VertexType) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = vertex;

	HRESULT hr;
	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));

	desc = { 0 };
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	data = { 0 };
	data.pSysMem = index;

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));

	return true;
}

void LensFlare::CreateState()
{
	D3D11_SAMPLER_DESC samplerDesc;

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	D3D::GetDevice()->CreateSamplerState(&samplerDesc, &sampleState);

	D3D11_BLEND_DESC blendStateDescription;

	// 블렌드 상태 설명을 지 웁니다.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// 알파 지원 블렌드 상태 설명을 만듭니다.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	D3D::GetDevice()->CreateBlendState(&blendStateDescription, &alphaBlendDisableState);

	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	
	D3D::GetDevice()->CreateBlendState(&blendStateDescription, &alphaBlendEnableState);

	D3D11_DEPTH_STENCIL_DESC depthDesc;
	States::GetDepthStencilDesc(&depthDesc);
	depthDesc.DepthEnable = true;
	States::CreateDepthStencil(&depthDesc, &depthOnState);

	States::GetDepthStencilDesc(&depthDesc);
	depthDesc.DepthEnable = false;
	States::CreateDepthStencil(&depthDesc, &depthOffState);
}

bool LensFlare::LoadTextures(wstring textureFilename)
{
	Texture::LoadTexture(textureFilename, &lensFlareTexture);

	return true;
}
