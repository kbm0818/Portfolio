#include "stdafx.h"
#include "SkyPlane.h"

SkyPlane::SkyPlane()
	:skyPlane(nullptr), vertexBuffer(nullptr), indexBuffer(nullptr)
	, cloudTexture1(nullptr), cloudTexture2(nullptr)
{
	skyPlaneBuffer = new SkyPlaneBuffer();
	worldBuffer = new WorldBuffer();
	shader = new Shader(Shaders + L"SkyPlane.hlsl", ShaderType::VP);

	CreateState();

	D3DXMatrixIdentity(&world);

	Initialize(Textures + L"cloud001.dds", Textures + L"cloud002.dds");
}

SkyPlane::~SkyPlane()
{
	SAFE_RELEASE(alphaBlendEnableState);
	SAFE_RELEASE(alphaBlendDisableState);
	SAFE_RELEASE(sampleState);

	SAFE_DELETE(skyPlaneBuffer);
	SAFE_DELETE(shader);

	SAFE_RELEASE(cloudTexture2);
	SAFE_RELEASE(cloudTexture1);

	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);

	SAFE_DELETE_ARRAY(skyPlane);
}

void SkyPlane::Update()
{
	// Increment the translation values to simulate the moving clouds.
	m_textureTranslation[0] += m_translationSpeed[0];
	m_textureTranslation[1] += m_translationSpeed[1];
	m_textureTranslation[2] += m_translationSpeed[2];
	m_textureTranslation[3] += m_translationSpeed[3];

	// Keep the values in the zero to one range.
	if (m_textureTranslation[0] > 1.0f) { m_textureTranslation[0] -= 1.0f; }
	if (m_textureTranslation[1] > 1.0f) { m_textureTranslation[1] -= 1.0f; }
	if (m_textureTranslation[2] > 1.0f) { m_textureTranslation[2] -= 1.0f; }
	if (m_textureTranslation[3] > 1.0f) { m_textureTranslation[3] -= 1.0f; }

	skyPlaneBuffer->Data.brightness = m_brightness;
	skyPlaneBuffer->Data.firstTranslationX = m_textureTranslation[0];
	skyPlaneBuffer->Data.firstTranslationZ = m_textureTranslation[1];
	skyPlaneBuffer->Data.secondTranslationX = m_textureTranslation[2];
	skyPlaneBuffer->Data.secondTranslationZ = m_textureTranslation[3];


}

void SkyPlane::Render()
{
	unsigned int stride;
	unsigned int offset;

	stride = sizeof(VertexType);
	offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	skyPlaneBuffer->SetPSBuffer(1);
	worldBuffer->SetVSBuffer(0);

	shader->Render(ShaderType::VP);

	D3D::GetDC()->PSSetShaderResources(0, 1, &cloudTexture1);
	D3D::GetDC()->PSSetShaderResources(1, 1, &cloudTexture2);

	D3D::GetDC()->PSSetSamplers(0, 1, &sampleState);

	float blendFactor[4];

	// 블렌드 인수를 설정합니다.
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;

	D3D::GetDC()->OMSetBlendState(alphaBlendEnableState, blendFactor, 0xffffffff);

	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);

	D3D::GetDC()->OMSetBlendState(alphaBlendDisableState, blendFactor, 0xffffffff);
}

void SkyPlane::PostRender(GuiSettings * settings)
{
	if (settings->bShowEnvironmentWindow == true)
	{
		ImGui::Begin("Environment");

		if (ImGui::CollapsingHeader("Cloud"))
		{
			ImGui::DragFloat4("CloudSpeed", m_translationSpeed, 0.001f, 0.0f, 1.0f,"%.5f");
			ImGui::SliderFloat("CloudBright", &m_brightness, 0.0f, 1.0f);
		}

		ImGui::End();
	}
}

bool SkyPlane::Initialize(wstring textureFilename1, wstring textureFilename2)
{
	int skyPlaneResolution, textureRepeat;
	float skyPlaneWidth, skyPlaneTop, skyPlaneBottom;
	bool result;

	// Set the sky plane parameters.
	skyPlaneResolution = 10;
	skyPlaneWidth = 10.0f;
	skyPlaneTop = 0.5f;
	skyPlaneBottom = 0.0f;
	textureRepeat = 4;

	// Set the brightness of the clouds.
	m_brightness = 0.65f;

	// Setup the cloud translation speed increments.
	m_translationSpeed[0] = 0.0003f;   // First texture X translation speed.
	m_translationSpeed[1] = 0.0f;      // First texture Z translation speed.
	m_translationSpeed[2] = 0.00015f;  // Second texture X translation speed.
	m_translationSpeed[3] = 0.0f;      // Second texture Z translation speed.

									   // Initialize the texture translation values.
	m_textureTranslation[0] = 0.0f;
	m_textureTranslation[1] = 0.0f;
	m_textureTranslation[2] = 0.0f;
	m_textureTranslation[3] = 0.0f;

	// Create the sky plane.
	result = CreateData(skyPlaneResolution, skyPlaneWidth, skyPlaneTop, skyPlaneBottom, textureRepeat);
	if (!result)
	{
		return false;
	}

	// Create the vertex and index buffer for the sky plane.
	result = InitializeBuffers(skyPlaneResolution);
	if (!result)
	{
		return false;
	}

	// Load the sky plane textures.
	result = LoadTextures(textureFilename1, textureFilename2);
	if (!result)
	{
		return false;
	}

	return true;
}

bool SkyPlane::CreateData(int skyPlaneResolution, float skyPlaneWidth, float skyPlaneTop, float skyPlaneBottom, int textureRepeat)
{
	float quadSize, radius, constant, textureDelta;
	int i, j, index;
	float positionX, positionY, positionZ, tu, tv;


	// Create the array to hold the sky plane coordinates.
	skyPlane = new SkyPlaneType[(skyPlaneResolution + 1) * (skyPlaneResolution + 1)];
	if (!skyPlane)
	{
		return false;
	}

	// Determine the size of each quad on the sky plane.
	quadSize = skyPlaneWidth / (float)skyPlaneResolution;

	// Calculate the radius of the sky plane based on the width.
	radius = skyPlaneWidth / 2.0f;

	// Calculate the height constant to increment by.
	constant = (skyPlaneTop - skyPlaneBottom) / (radius * radius);

	// Calculate the texture coordinate increment value.
	textureDelta = (float)textureRepeat / (float)skyPlaneResolution;

	// Loop through the sky plane and build the coordinates based on the increment values given.
	for (j = 0; j <= skyPlaneResolution; j++)
	{
		for (i = 0; i <= skyPlaneResolution; i++)
		{
			// Calculate the vertex coordinates.
			positionX = (-0.5f * skyPlaneWidth) + ((float)i * quadSize);
			positionZ = (-0.5f * skyPlaneWidth) + ((float)j * quadSize);
			positionY = skyPlaneTop - (constant * ((positionX * positionX) + (positionZ * positionZ)));

			// Calculate the texture coordinates.
			tu = (float)i * textureDelta;
			tv = (float)j * textureDelta;

			// Calculate the index into the sky plane array to add this coordinate.
			index = j * (skyPlaneResolution + 1) + i;

			// Add the coordinates to the sky plane array.
			skyPlane[index].x = positionX;
			skyPlane[index].y = positionY;
			skyPlane[index].z = positionZ;
			skyPlane[index].tu = tu;
			skyPlane[index].tv = tv;
		}
	}

	return true;
}

bool SkyPlane::InitializeBuffers(int skyPlaneResolution)
{
	VertexType* vertices;
	UINT* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i, j, index, index1, index2, index3, index4;


	// Calculate the number of vertices in the sky plane mesh.
	vertexCount = (skyPlaneResolution + 1) * (skyPlaneResolution + 1) * 6;

	// Set the index count to the same as the vertex count.
	indexCount = vertexCount;

	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new UINT[indexCount];
	if (!indices)
	{
		return false;
	}

	// Initialize the index into the vertex array.
	index = 0;

	// Load the vertex and index array with the sky plane array data.
	for (j = 0; j<skyPlaneResolution; j++)
	{
		for (i = 0; i<skyPlaneResolution; i++)
		{
			index1 = j * (skyPlaneResolution + 1) + i;
			index2 = j * (skyPlaneResolution + 1) + (i + 1);
			index3 = (j + 1) * (skyPlaneResolution + 1) + i;
			index4 = (j + 1) * (skyPlaneResolution + 1) + (i + 1);

			// Triangle 1 - Upper Left
			vertices[index].position = D3DXVECTOR3(skyPlane[index1].x, skyPlane[index1].y, skyPlane[index1].z);
			vertices[index].uv = D3DXVECTOR2(skyPlane[index1].tu, skyPlane[index1].tv);
			indices[index] = index;
			index++;

			// Triangle 1 - Upper Right
			vertices[index].position = D3DXVECTOR3(skyPlane[index2].x, skyPlane[index2].y, skyPlane[index2].z);
			vertices[index].uv = D3DXVECTOR2(skyPlane[index2].tu, skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// Triangle 1 - Bottom Left
			vertices[index].position = D3DXVECTOR3(skyPlane[index3].x, skyPlane[index3].y, skyPlane[index3].z);
			vertices[index].uv = D3DXVECTOR2(skyPlane[index3].tu, skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// Triangle 2 - Bottom Left
			vertices[index].position = D3DXVECTOR3(skyPlane[index3].x, skyPlane[index3].y, skyPlane[index3].z);
			vertices[index].uv = D3DXVECTOR2(skyPlane[index3].tu, skyPlane[index3].tv);
			indices[index] = index;
			index++;

			// Triangle 2 - Upper Right
			vertices[index].position = D3DXVECTOR3(skyPlane[index2].x, skyPlane[index2].y, skyPlane[index2].z);
			vertices[index].uv = D3DXVECTOR2(skyPlane[index2].tu, skyPlane[index2].tv);
			indices[index] = index;
			index++;

			// Triangle 2 - Bottom Right
			vertices[index].position = D3DXVECTOR3(skyPlane[index4].x, skyPlane[index4].y, skyPlane[index4].z);
			vertices[index].uv = D3DXVECTOR2(skyPlane[index4].tu, skyPlane[index4].tv);
			indices[index] = index;
			index++;
		}
	}

	// Set up the description of the vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	result = D3D::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = D3D::GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	SAFE_DELETE(vertices);
	SAFE_DELETE(indices);
	
	return true;
}

void SkyPlane::CreateState()
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
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	D3D::GetDevice()->CreateBlendState(&blendStateDescription, &alphaBlendEnableState);


}

bool SkyPlane::LoadTextures(wstring textureFilename1, wstring textureFilename2)
{
	Texture::LoadTexture(textureFilename1, &cloudTexture1);
	Texture::LoadTexture(textureFilename2, &cloudTexture2);

	return false;
}