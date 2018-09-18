#include "stdafx.h"
#include "BinaryMesh.h"
#include "BinaryFile.h"

BinaryMesh::BinaryMesh()
	: vertexData(NULL)
	, vertexCount(0)
	, indexData(NULL)
	, indexCount(0)
{
}

BinaryMesh::~BinaryMesh()
{
	SAFE_DELETE_ARRAY(vertexData);
	SAFE_DELETE_ARRAY(indexData);
}

void BinaryMesh::Open(wstring filePath)
{
	BinaryReader reader;
	reader.Open(filePath);

	vertexCount = reader.UInt();
	vertexData = new VertexTextureNormal[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].position = reader.Vector3();
		vertexData[i].uv = reader.Vector2();
		vertexData[i].normal = reader.Vector3();
	}

	indexCount = reader.UInt();
	indexData = new UINT[indexCount];
	for (UINT i = 0; i < indexCount; i++)
		indexData[i] = reader.UInt();

	reader.Close();
}

void BinaryMesh::CopyVertex(Vertex ** vertex, UINT * count)
{
	*count = vertexCount;

	*vertex = new Vertex[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
		(*vertex)[i].position = vertexData[i].position;
}

void BinaryMesh::CopyVertex(VertexTexture ** vertex, UINT * count)
{
	*count = vertexCount;

	*vertex = new VertexTexture[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		(*vertex)[i].position = vertexData[i].position;
		(*vertex)[i].uv = vertexData[i].uv;
	}
}

void BinaryMesh::CopyVertex(VertexColor ** vertex, UINT * count, const D3DXCOLOR & color)
{
	*count = vertexCount;

	*vertex = new VertexColor[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		(*vertex)[i].position = vertexData[i].position;
		(*vertex)[i].color = color;
	}
}

void BinaryMesh::CopyVertex(VertexColorNormal ** vertex, UINT * count, const D3DXCOLOR & color)
{
	*count = vertexCount;

	*vertex = new VertexColorNormal[vertexCount];
	for (UINT i = 0; i < vertexCount; i++)
	{
		(*vertex)[i].position = vertexData[i].position;
		(*vertex)[i].normal = vertexData[i].normal;
		(*vertex)[i].color = color;
	}
}

void BinaryMesh::CopyVertex(VertexTextureNormal ** vertex, UINT * count)
{
	*count = vertexCount;

	*vertex = new VertexTextureNormal[vertexCount];
	memcpy(*vertex, vertexData, sizeof(VertexTextureNormal) * vertexCount);
}

void BinaryMesh::CopyIndex(UINT ** index, UINT * count)
{
	*count = indexCount;

	*index = new UINT[indexCount];
	memcpy(*index, indexData, sizeof(UINT) * indexCount);
}
