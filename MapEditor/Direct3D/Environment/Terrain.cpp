#include "../stdafx.h"
#include "Terrain.h"
#include "TerrainBuffer.h"
#include "Brush.h"


Terrain::Terrain()
	:width(255), height(255)
	, diffuseMapFile(L""), stage1MapFile(L""), stage2MapFile(L""), stage3MapFile(L""), stage4MapFile(L"")
	, diffuseMap(nullptr), stage1Map(nullptr), stage2Map(nullptr), stage3Map(nullptr), stage4Map(nullptr)
	, vertexBuffer(nullptr), indexBuffer(nullptr)
	, vertexCount(0), indexCount(0)
	, shader(nullptr), terrainBuffer(nullptr)
	, brush(nullptr)
{
	D3DXMatrixIdentity(&world);
	terrainBuffer = new TerrainBuffer();

	CreateSRV();
	CreateVertexData();
	CalcNormalVector();
	CreateIndexData();
	CreateBuffer();

	brush = new Brush(this);
}

Terrain::~Terrain()
{
	SAFE_DELETE(brush);
	SAFE_DELETE(terrainBuffer);

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
	
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(vertexBuffer);
}

void Terrain::CreateVertexData()
{
	if (width <= 0 || height <= 0)
		return;

	vertexCount = (width + 1) * (height + 1);
	vertexData = new VertexType[vertexCount];

	UINT index = 0;
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			vertexData[index].position.x = (float)x;
			vertexData[index].position.y = 0.0f;
			vertexData[index].position.z = (float)z;

			vertexData[index].uv.x = x / (float)width;
			vertexData[index].uv.y = z / (float)height;
			
			if(x % 2 == 0)
				vertexData[index].uv2.x = 0.0f;
			else
				vertexData[index].uv2.x = 1.0f;
			if (z % 2 == 0)
				vertexData[index].uv2.y = 0.0f;
			else
				vertexData[index].uv2.y = 1.0f;

			vertexData[index].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			vertexData[index].color = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

			index++;
		}
	}
}

void Terrain::CreateIndexData()
{
	if (width <= 0 || height <= 0)
		return;

	indexCount = width * height * 6;
	indexData = new UINT[indexCount];

	UINT count = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indexData[count + 0] = (width + 1) * z + x;
			indexData[count + 1] = (width + 1) * (z + 1) + x;
			indexData[count + 2] = (width + 1) * z + x + 1;
			indexData[count + 3] = (width + 1) * z + x + 1;
			indexData[count + 4] = (width + 1) * (z + 1) + x;
			indexData[count + 5] = (width + 1) * (z + 1) + (x + 1);
			
			count += 6;
		}//for(x)
	}//for(z)
}

void Terrain::CalcNormalVector()
{
	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	for (UINT i = 0; i < (indexCount / 3); i++)
	{
		UINT index0 = indexData[i * 3 + 0];
		UINT index1 = indexData[i * 3 + 1];
		UINT index2 = indexData[i * 3 + 2];

		VertexType v0 = vertexData[index0];
		VertexType v1 = vertexData[index1];
		VertexType v2 = vertexData[index2];

		D3DXVECTOR3 x = v1.position - v0.position;
		D3DXVECTOR3 y = v2.position - v0.position;

		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &x, &y);

		vertexData[index0].normal += normal;
		vertexData[index1].normal += normal;
		vertexData[index2].normal += normal;
	}
}

void Terrain::CreateBuffer()
{
	if (width <= 0 || height <= 0)
		return;

	HRESULT hr;

	D3D11_BUFFER_DESC vertexDesc = { 0 };
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResource = { 0 };
	vertexSubResource.pSysMem = vertexData;

	hr = D3D::GetDevice()->CreateBuffer(&vertexDesc, &vertexSubResource, &vertexBuffer);
	assert(SUCCEEDED(hr));


	D3D11_BUFFER_DESC indexDesc = { 0 };
	indexDesc.Usage = D3D11_USAGE_DEFAULT;
	indexDesc.ByteWidth = sizeof(UINT) * indexCount;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubResource = { 0 };
	indexSubResource.pSysMem = indexData;

	hr = D3D::GetDevice()->CreateBuffer(&indexDesc, &indexSubResource, &indexBuffer);
	assert(SUCCEEDED(hr));
}

void Terrain::CreateSRV()
{
	CreateSRV(diffuseMapFile, &diffuseMap);
	CreateSRV(stage1MapFile, &stage1Map);
	CreateSRV(stage2MapFile, &stage2Map);
	CreateSRV(stage3MapFile, &stage3Map);
	CreateSRV(stage4MapFile, &stage4Map);
}

void Terrain::CreateSRV(wstring file, ID3D11ShaderResourceView ** view)
{
	SAFE_RELEASE(*view);

	if (wcscmp(file.c_str(), L"") != 0)
	{
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			D3D::GetDevice()
			, file.c_str()
			, NULL
			, NULL
			, view
			, NULL
		);
		assert(SUCCEEDED(hr));
	}
}

bool Terrain::SaveBMPfile(char* fileName, RGBQUAD* pImage)
{
	// DIB의 형식을 정의한다.
	BITMAPINFO dib_define;
	dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dib_define.bmiHeader.biWidth = width+1;
	dib_define.bmiHeader.biHeight = height+1;
	dib_define.bmiHeader.biPlanes = 1;
	dib_define.bmiHeader.biBitCount = 24;
	dib_define.bmiHeader.biCompression = BI_RGB;
	dib_define.bmiHeader.biSizeImage = (width + 1) * (height + 1) * 3;
	dib_define.bmiHeader.biXPelsPerMeter = 0;
	dib_define.bmiHeader.biYPelsPerMeter = 0;
	dib_define.bmiHeader.biClrImportant = 0;
	dib_define.bmiHeader.biClrUsed = 0;

	// DIB 파일의 헤더 내용을 구성한다.
	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));

	dib_format_layout.bfType = (WORD)(('M' << 8 | 'B'));
	dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * 256);
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + ((256 - 1) * sizeof(RGBQUAD) + sizeof(BITMAPINFO) + dib_define.bmiHeader.biSizeImage);
	dib_format_layout.bfReserved1 = 0;
	dib_format_layout.bfReserved2 = 0;

	// 파일 생성.
	FILE* fp = nullptr;
	fopen_s(&fp, fileName, "wb");
	if (nullptr == fp)
	{
		//쓰기실패인데
		return false;
	}

	// 생성 후 헤더 및 데이터 쓰기.
	fwrite(&dib_format_layout, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&dib_define, sizeof(BITMAPINFO), 1, fp);
	fwrite(pImage, 1, dib_define.bmiHeader.biSizeImage, fp);
	fclose(fp);

	return true;
}

bool Terrain::SaveBMPFile(int bmp_wid, int bmp_hig, RGBQUAD * bmp_data, int flag, LPCWSTR str)
{
	// 24bit
	if (flag == 24)
	{
		/***********************************************************************************************/
		BITMAPINFO        szBitmapInfo;
		szBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);                // bmiHeader의 구조체 크기 설정
		szBitmapInfo.bmiHeader.biWidth = bmp_wid;                                // 이미지 가로 크기
		szBitmapInfo.bmiHeader.biHeight = bmp_hig;                                // 이미지 세로 크기
		szBitmapInfo.bmiHeader.biPlanes = 1;                                        // 비트플렌 = 1
		szBitmapInfo.bmiHeader.biBitCount = 24;                                        // 픽셀당 비트수, 트루컬러 = 24
		szBitmapInfo.bmiHeader.biCompression = BI_RGB;                        // 압축없음
		szBitmapInfo.bmiHeader.biSizeImage = bmp_wid*bmp_hig * 3;        // 그림크기 hig * wid * 3
		szBitmapInfo.bmiHeader.biXPelsPerMeter = 0;
		szBitmapInfo.bmiHeader.biYPelsPerMeter = 0;
		szBitmapInfo.bmiHeader.biClrUsed = 0;
		szBitmapInfo.bmiHeader.biClrImportant = 0;
		/***********************************************************************************************/

		DWORD        size, len;
		BITMAPFILEHEADER        bmpHeader;
		int m_nColor = 256;

		bmpHeader.bfType = (WORD)(('M' << 8 | 'B'));
		bmpHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + ((m_nColor) * sizeof(RGBQUAD));
		bmpHeader.bfSize = sizeof(bmpHeader) + ((m_nColor - 1) * sizeof(RGBQUAD) + sizeof(BITMAPINFO) + szBitmapInfo.bmiHeader.biSizeImage);
		bmpHeader.bfReserved1 = 0;
		bmpHeader.bfReserved2 = 0;

		HANDLE fd = CreateFile(str, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (!WriteFile(fd, (LPSTR)&bmpHeader, sizeof(bmpHeader), &len, NULL))                return false;

		size = ((m_nColor - 1) * sizeof(RGBQUAD)) + sizeof(BITMAPINFO);

		if (!WriteFile(fd, &szBitmapInfo, size, &len, NULL))                                                return false;
		if (!WriteFile(fd, bmp_data, szBitmapInfo.bmiHeader.biSizeImage, &len, NULL))        return false;

		CloseHandle(fd);

		return true;
	}
}

void Terrain::Update(Camera* camera)
{
	if (shader == nullptr || vertexBuffer == nullptr)
		return;

	brush->Update(camera);
}

void Terrain::Render(Camera* camera)
{
	if (shader == nullptr || vertexBuffer == nullptr)
		return;

	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D11ShaderResourceView* views[5];
	views[0] = diffuseMap;
	views[1] = stage1Map;
	views[2] = stage2Map;
	views[3] = stage3Map;
	views[4] = stage4Map;

	D3D::GetDC()->PSSetShaderResources(0, 5, views);

	camera->SetVSBuffer(&world);
	terrainBuffer->SetPSBuffer(1);
	brush->SetPSBuffer(2);
	brush->SetPSGridBuffer(3);

	shader->Render();
	
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
}

void Terrain::SetShader(wstring file)
{
	SAFE_DELETE(shader);

	shader = new Shader(file);
	shader->CreateInputLayout(VertexType::desc, VertexType::count);
}

UINT Terrain::GetWidth()
{
	return width;
}

UINT Terrain::GetHeight()
{
	return height;
}

void Terrain::SetWidth(UINT width)
{
	this->width = width;

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	CreateVertexData();
	CreateIndexData();
	CreateBuffer();
}

void Terrain::SetHeight(UINT height)
{
	this->height = height;

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	CreateVertexData();
	CreateIndexData();
	CreateBuffer();
}

void Terrain::SetSize(UINT width, UINT height)
{
	this->width = width;
	this->height = height;

	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	CreateVertexData();
	CreateIndexData();
	CreateBuffer();
}

UINT Terrain::GetDiffuseColor()
{
	return (UINT)(terrainBuffer->GetData().diffuse);
}

UINT Terrain::GetAmbientColor()
{
	return (UINT)(terrainBuffer->GetData().ambient);
}

void Terrain::SetDiffuseColor(int diffuse)
{
	D3DXCOLOR color = D3DXCOLOR(diffuse);

	terrainBuffer->SetDiffuseColor(color);
}

void Terrain::SetAmbientColor(int ambient)
{
	D3DXCOLOR color = D3DXCOLOR(ambient);

	terrainBuffer->SetAmbientColor(color);
}

void Terrain::SetDiffuseFile(wstring file)
{
	diffuseMapFile = file;

	CreateSRV(diffuseMapFile, &diffuseMap);
}

void Terrain::SetStage1File(wstring file)
{
	stage1MapFile = file;

	CreateSRV(stage1MapFile, &stage1Map);
}

void Terrain::SetStage2File(wstring file)
{
	stage2MapFile = file;

	CreateSRV(stage2MapFile, &stage2Map);
}

void Terrain::SetStage3File(wstring file)
{
	stage3MapFile = file;

	CreateSRV(stage3MapFile, &stage3Map);
}

void Terrain::SetStage4File(wstring file)
{
	stage4MapFile = file;

	CreateSRV(stage4MapFile, &stage4Map);
}

Brush * Terrain::GetBrush()
{
	return brush;
}

bool Terrain::SaveTerrain()
{
	//return SaveTerrainTexture(); 그림파일로 출력
	return SaveTerrainBinary();
}

bool Terrain::SaveTerrainTexture()
{
	RGBQUAD* heightData = new RGBQUAD[vertexCount];
	RGBQUAD* blendData = new RGBQUAD[vertexCount];
	RGBQUAD* normalData = new RGBQUAD[vertexCount];

	union change {
		float val;
		byte val2[4];
	};

	for (UINT i = 0; i < vertexCount; i++)
	{
		union change uc;
		uc.val = vertexData[i].position.y;

		heightData[i].rgbRed = uc.val2[0];
		heightData[i].rgbGreen = uc.val2[1];
		heightData[i].rgbBlue = uc.val2[2];
		heightData[i].rgbReserved = 0;

		D3DXVec3Normalize(&(vertexData[i].normal), &(vertexData[i].normal));

		uc.val = ((vertexData[i].normal.x + 1.0f) / 2);
		normalData[i].rgbRed = uc.val2[0];
		uc.val = ((vertexData[i].normal.y + 1.0f) / 2);
		normalData[i].rgbGreen = uc.val2[0];
		uc.val = ((vertexData[i].normal.z + 1.0f) / 2);
		normalData[i].rgbBlue = uc.val2[0];
		normalData[i].rgbReserved = 0;

		blendData[i].rgbRed = (byte)(vertexData[i].color.b);
		blendData[i].rgbGreen = (byte)(vertexData[i].color.g);
		blendData[i].rgbBlue = (byte)(vertexData[i].color.r);
		blendData[i].rgbReserved = 0;
	}

	//SaveBMPfile("../../_Contents/HeightMaps/heightTest.bmp", heightData);
	//SaveBMPfile("../../_Contents/HeightMaps/normalTest.bmp", normalData);
	//SaveBMPfile("../../_Contents/HeightMaps/blendTest.bmp", blendData);

	SaveBMPFile(width + 1, height + 1, heightData, 24, L"../../_Contents/HeightMaps/heightTest.bmp");
	SaveBMPFile(width + 1, height + 1, normalData, 24, L"../../_Contents/HeightMaps/normalTest.bmp");
	SaveBMPFile(width + 1, height + 1, blendData, 24, L"../../_Contents/HeightMaps/blendTest.bmp");

	SAFE_DELETE_ARRAY(heightData);
	SAFE_DELETE_ARRAY(blendData);
	SAFE_DELETE_ARRAY(normalData);

	return true;
}

bool Terrain::SaveTerrainBinary()
{
	BinaryWriter* writer = new BinaryWriter();

	writer->Open(Contents + L"HeightMaps/heightTest.map");
	{
		writer->Write(vertexCount);
		
		for (UINT i = 0; i < vertexCount; i++)
		{
			writer->Write(vertexData[i].position.x);
			writer->Write(vertexData[i].position.y);
			writer->Write(vertexData[i].position.z);

			writer->Write(vertexData[i].normal.x);
			writer->Write(vertexData[i].normal.y);
			writer->Write(vertexData[i].normal.z);

			writer->Write(vertexData[i].color.a);
			writer->Write(vertexData[i].color.r);
			writer->Write(vertexData[i].color.g);
			writer->Write(vertexData[i].color.b);
		}

		writer->Write(indexCount);
		for (UINT i = 0; i < indexCount; i++)
		{
			writer->Write(indexData[i]);
		}
	}
	writer->Close();

	return true;
}
