#include "stdafx.h"
#include "Font.h"
#include "../Viewer/Orthographic.h"

#include <fstream>
#include <sstream>

Font::Font(ExecuteValues * values, wstring file, wstring texture)
	: values(values)
	, test("Park Mun Su!")
	, testLocation(0, 100)
	, bRemap(true)
{
	shader = new Shader(Shaders + L"999_Font.hlsl");
	worldBuffer = new WorldBuffer();

	D3DXMatrixIdentity(&view);
	{
		D3DDesc desc;
		D3D::GetDesc(&desc);

		ortho = new Orthographic(0, desc.Width, desc.Height, 0);
	}

	CreateBuffer();
	Parse(file, texture);

	{
		D3D11_BLEND_DESC desc;
		States::GetBlendDesc(&desc);
		States::CreateBlend(&desc, &blendState[0]);

		desc.RenderTarget[0].BlendEnable = true;
		States::CreateBlend(&desc, &blendState[1]);
	}

	{
		D3D11_DEPTH_STENCIL_DESC desc;
		States::GetDepthStencilDesc(&desc);
		States::CreateDepthStencil(&desc, &depthState[0]);

		desc.DepthEnable = false;
		States::CreateDepthStencil(&desc, &depthState[1]);
	}


	D3DX11_IMAGE_LOAD_INFO info;
	info.Width = fontDesc.Width;
	info.Height = fontDesc.Height;

	HRESULT hr = D3DX11CreateShaderResourceViewFromFile
	(
		D3D::GetDevice(), texture.c_str(), &info, NULL, &srv, NULL
	);
	assert(SUCCEEDED(hr));
}

Font::~Font()
{
	SAFE_DELETE(shader);
	SAFE_DELETE(worldBuffer);

	SAFE_DELETE(ortho);

	SAFE_RELEASE(blendState[0]);
	SAFE_RELEASE(blendState[1]);

	SAFE_RELEASE(depthState[0]);
	SAFE_RELEASE(depthState[1]);

	SAFE_DELETE_ARRAY(vertices);
	SAFE_DELETE_ARRAY(indices);

	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

void Font::Update()
{
	if (bRemap == true)
	{
		bRemap = false;

		for (size_t i = 0; i < test.size(); i++)
		{
			UINT id = test[i];

			float CharX = (float)fontDesc.desc[id].X;
			float CharY = (float)fontDesc.desc[id].Y;
			float Width = (float)fontDesc.desc[id].Width;
			float Height = (float)fontDesc.desc[id].Height;
			float OffsetX = fontDesc.desc[id].OffsetX;
			float OffsetY = fontDesc.desc[id].OffsetY;

			//lower left
			vertices[i * 4 + 0].position.x = testLocation.x + OffsetX;
			vertices[i * 4 + 0].position.y = testLocation.y + Height + OffsetY;
			vertices[i * 4 + 0].uv.x = CharX / fontDesc.Width;
			vertices[i * 4 + 0].uv.y = (CharY + Height) / fontDesc.Height;

			//upper left
			vertices[i * 4 + 1].position.x = testLocation.x + OffsetX;
			vertices[i * 4 + 1].position.y = testLocation.y + OffsetY;
			vertices[i * 4 + 1].uv.x = CharX / fontDesc.Width;
			vertices[i * 4 + 1].uv.y = CharY / fontDesc.Height;

			//lower right
			vertices[i * 4 + 2].position.x = testLocation.x + Width + OffsetX;
			vertices[i * 4 + 2].position.y = testLocation.y + Height + OffsetY;
			vertices[i * 4 + 2].uv.x = (CharX + Width) / fontDesc.Width;
			vertices[i * 4 + 2].uv.y = (CharY + Height) / fontDesc.Height;

			//upper right
			vertices[i * 4 + 3].position.x = testLocation.x + Width + OffsetX;
			vertices[i * 4 + 3].position.y = testLocation.y + OffsetY;
			vertices[i * 4 + 3].uv.x = (CharX + Width) / fontDesc.Width;
			vertices[i * 4 + 3].uv.y = CharY / fontDesc.Height;

			testLocation.x += fontDesc.desc[id].AdvanceX;
		}//for(i)

		D3D11_MAPPED_SUBRESOURCE subResource;
		HRESULT hr = D3D::GetDC()->Map
		(
			vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);

		memcpy(subResource.pData, vertices, sizeof(VertexTexture) * MaxSize * 4);

		D3D::GetDC()->Unmap(vertexBuffer, 0);
	}//if(bRemap)
}

void Font::Render()
{
	UINT stride = sizeof(VertexTexture);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	D3DXMATRIX projection;
	ortho->GetMatrix(&projection);

	values->ViewProjection->SetView(view);
	values->ViewProjection->SetProjection(projection);
	values->ViewProjection->SetVSBuffer(0);

	worldBuffer->SetVSBuffer(1);

	D3D::GetDC()->PSSetShaderResources(5, 1, &srv);

	shader->Render();

	D3D::GetDC()->OMSetDepthStencilState(depthState[1], 1);
	D3D::GetDC()->OMSetBlendState(blendState[1], NULL, 0xFF);
	D3D::GetDC()->DrawIndexed(test.size() * 6, 0, 0);
	D3D::GetDC()->OMSetBlendState(blendState[0], NULL, 0xFF);
	D3D::GetDC()->OMSetDepthStencilState(depthState[0], 1);
}

void Font::CreateBuffer()
{
	vertices = new VertexTexture[MaxSize * 4];
	for (UINT i = 0; i < MaxSize; i++)
	{
		vertices[i * 4 + 0].position = D3DXVECTOR3(0, 0, 0);
		vertices[i * 4 + 1].position = D3DXVECTOR3(0, 0, 0);
		vertices[i * 4 + 2].position = D3DXVECTOR3(0, 0, 0);
		vertices[i * 4 + 3].position = D3DXVECTOR3(0, 0, 0);

		vertices[i * 4 + 0].uv = D3DXVECTOR2(0, 1);
		vertices[i * 4 + 1].uv = D3DXVECTOR2(0, 0);
		vertices[i * 4 + 2].uv = D3DXVECTOR2(1, 1);
		vertices[i * 4 + 3].uv = D3DXVECTOR2(1, 0);
	}

	indices = new UINT[MaxSize * 6];
	for (UINT i = 0; i < MaxSize; i++)
	{
		indices[i * 6 + 0] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;

		indices[i * 6 + 3] = i * 4 + 2;
		indices[i * 6 + 4] = i * 4 + 1;
		indices[i * 6 + 5] = i * 4 + 3;
	}


	//VertexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = sizeof(VertexTexture) * MaxSize * 4;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = vertices;

		HRESULT hr;
		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	//IndexBuffer
	{
		D3D11_BUFFER_DESC desc = { 0 };
		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.ByteWidth = sizeof(UINT) * MaxSize * 6;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA data = { 0 };
		data.pSysMem = indices;

		HRESULT hr;
		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
		assert(SUCCEEDED(hr));
	}
}

void Font::Parse(wstring file, wstring texture)
{
	size_t count;
	string line, read, key, value;

	ifstream stream;
	stream.open(file);
	while (stream.eof() == false)
	{
		stringstream lineStream;
		getline(stream, line);
		lineStream << line;

		lineStream >> read;
		if (read == "common")
		{
			while (lineStream.eof() == false)
			{
				lineStream >> read;

				count = read.find('=');
				key = read.substr(0, count);
				value = read.substr(count + 1);
			
				stringstream convert;
				convert << value;
				
				if (key == "lineHeight") convert >> fontDesc.LineHeight;
				if (key == "base") convert >> fontDesc.Base;
				if (key == "scaleW") convert >> fontDesc.Width;
				if (key == "scaleH") convert >> fontDesc.Height;
				if (key == "pages") convert >> fontDesc.PageCount;
			}//while(lineStream)
		}
		else if (read == "char")
		{
			USHORT id = 0;

			while (lineStream.eof() == false)
			{
				lineStream >> read;

				count = read.find('=');
				key = read.substr(0, count);
				value = read.substr(count + 1);

				stringstream convert;
				convert << value;

				if (key == "id") convert >> id;
				else if (key == "x") convert >> fontDesc.desc[id].X;
				else if (key == "y") convert >> fontDesc.desc[id].Y;
				else if (key == "width") convert >> fontDesc.desc[id].Width;
				else if (key == "height") convert >> fontDesc.desc[id].Height;
				else if (key == "xoffset") convert >> fontDesc.desc[id].OffsetX;
				else if (key == "yoffset") convert >> fontDesc.desc[id].OffsetY;
				else if (key == "xadvance") convert >> fontDesc.desc[id].AdvanceX;
				else if (key == "page") convert >> fontDesc.desc[id].Page;
			}//while(lineStream)
		}//if(read)
	}

	stream.close();
}
