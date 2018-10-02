#pragma once
#include "./Utilities/Mesh.h"

template <typename T>
class MeshData : public IMeshData
{
protected:
	typedef T VertexType;

public:
	MeshData();
	virtual ~MeshData();

	virtual void Update(GlobalValues* values);
	void Initalize(MeshType type, wstring fileName, D3D11_USAGE vertexBufferUsage = D3D11_USAGE_DEFAULT, D3D11_USAGE indexBufferUsage = D3D11_USAGE_DEFAULT);
	virtual void Draw(GlobalValues* values, D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	virtual void SetDrawOption(int op) { meshRenderOption = op; };

protected:
	void ReadBinaryMeshFile(wstring fileName);
	void ReadBinaryFBXFile(wstring fileName);
	void ReadBinaryCustumFile(wstring fileName);
	void ReadFBXFile(wstring fileName);

	void CreateData(MeshType type, wstring fileName);
	void CreateBuffer(D3D11_USAGE vertexBufferUsage, D3D11_USAGE indexBufferUsage);
	void CalcTangentBinormal();
	void CalcNormal();

	void CopyBinaryMeshFile(wstring path);

	void OnOption(GlobalValues* values);
	void OffOption(GlobalValues* values);

protected:
	friend class TerrainMeshFactory;

	UINT vertexCount, indexCount;
	vector<VertexType> vertexData;
	vector<UINT> indexData;

	ID3D11Buffer *vertexBuffer, *indexBuffer;

	unsigned int meshRenderOption;
};

template<typename T>
inline MeshData<T>::MeshData()
	: vertexCount(0), indexCount(0), meshRenderOption(0)
	, vertexBuffer(nullptr), indexBuffer(nullptr)
{
}

template<typename T>
inline MeshData<T>::~MeshData()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);

	vertexData.clear();
	indexData.clear();
}

template<typename T>
inline void MeshData<T>::Update(GlobalValues * values)
{
}

template<typename T>
inline void MeshData<T>::Initalize(MeshType type, wstring fileName, D3D11_USAGE vertexBufferUsage, D3D11_USAGE indexBufferUsage)
{
	CreateData(type, fileName);
	CreateBuffer(vertexBufferUsage, indexBufferUsage);
}

template<typename T>
inline void MeshData<T>::Draw(GlobalValues* values, D3D_PRIMITIVE_TOPOLOGY primitiveTopology)
{
	UINT stride = sizeof(VertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(primitiveTopology);

	OnOption(values);
	D3D::GetDC()->DrawIndexed(indexCount, 0, 0);
	OffOption(values);
}

template<typename T>
inline void MeshData<T>::CreateData(MeshType type, wstring fileName)
{
	switch (type)
	{
	case MeshType::BinaryCustumFile:
		ReadBinaryCustumFile(fileName);
		break;
	case MeshType::BinaryFBXFile:
		ReadBinaryFBXFile(fileName);
		break;
	case MeshType::BinaryMeshFile:
		CopyBinaryMeshFile(fileName);
		break;
	case MeshType::FBXFile:
		ReadFBXFile(fileName);
		break;
	}
}

template<typename T>
inline void MeshData<T>::CreateBuffer(D3D11_USAGE vertexBufferUsage, D3D11_USAGE indexBufferUsage)
{
	HRESULT hr;

	D3D11_BUFFER_DESC desc = { 0 };
	desc.Usage = vertexBufferUsage;
	desc.ByteWidth = sizeof(VertexType) * vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data = { 0 };
	data.pSysMem = &vertexData[0];

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &vertexBuffer);
	assert(SUCCEEDED(hr));


	desc = { 0 };
	desc.Usage = indexBufferUsage;
	desc.ByteWidth = sizeof(UINT) * indexCount;
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	data = { 0 };
	data.pSysMem = &indexData[0];

	hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &indexBuffer);
	assert(SUCCEEDED(hr));
}

template<typename T>
inline void MeshData<T>::CalcTangentBinormal()
{
	for (UINT i = 0; i < indexCount;)
	{
		T &vertex1 = vertexData[indexData[i++]];
		T &vertex2 = vertexData[indexData[i++]];
		T &vertex3 = vertexData[indexData[i++]];

		D3DXVECTOR3 vector1 = vertex2.position - vertex1.position;
		D3DXVECTOR3 vector2 = vertex3.position - vertex1.position;

		D3DXVECTOR2 uv0 = vertex2.uv - vertex1.uv;
		D3DXVECTOR2 uv1 = vertex3.uv - vertex1.uv;

		float den = 1.0f / (uv0.x * uv1.y - uv1.x * uv0.y);

		D3DXVECTOR3 tangent = (uv1.y * vector1 - uv0.y * vector2) * den;
		D3DXVECTOR3 binormal = (uv0.x * vector2 - uv1.x * vector1) * den;

		D3DXVec3Normalize(&tangent, &tangent);
		D3DXVec3Normalize(&binormal, &binormal);

		vertex1.tangent += tangent;
		vertex2.tangent += tangent;
		vertex3.tangent += tangent;

		vertex1.binormal += binormal;
		vertex2.binormal += binormal;
		vertex3.binormal += binormal;
	}

	for (UINT i = 0; i < indexCount; i++)
	{
		T &V = vertexData[indexData[i]];

		D3DXVec3Normalize(&V.tangent, &V.tangent);
		D3DXVec3Normalize(&V.binormal, &V.binormal);
	}
}

template<typename T>
inline void MeshData<T>::CalcNormal()
{
	for (UINT i = 0; i < indexCount; i++)
	{
		T &V = vertexData[indexData[i]];
		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &V.tangent, &V.binormal);

		V.normal += normal;
	}

	for (UINT i = 0; i < indexCount; i++)
	{
		T &V = vertexData[indexData[i]];

		D3DXVec3Normalize(&V.normal, &V.normal);
	}
}


template<typename T>
inline void MeshData<T>::CopyBinaryMeshFile(wstring path)
{
	Mesh mesh;
	VertexTexture* tempV;
	UINT* tempI;

	mesh.Open(path);
	mesh.CopyVertex(&tempV, &vertexCount);
	mesh.CopyIndex(&tempI, &indexCount);

	vertexData.resize(vertexCount);
	indexData.resize(indexCount);

	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].position = tempV[i].position;
	}
	for (UINT i = 0; i < indexCount; i++)
	{
		indexData[i] = tempI[i];
	}

	SAFE_DELETE_ARRAY(tempV);
	SAFE_DELETE_ARRAY(tempI);
}

template<typename T>
inline void MeshData<T>::OnOption(GlobalValues* values)
{
	if (meshRenderOption == 0)
		return;

	unsigned int partOption = meshRenderOption & 15;
	if (partOption == (unsigned int)MeshRenderOption::R_CullReverce)
	{
		values->RenderOption->TurnOnCountCullMode();
	}
	else if (partOption == (unsigned int)MeshRenderOption::R_CullOff)
	{
		values->RenderOption->TurnOnAllCullMode();
	}
	else if (partOption == (unsigned int)MeshRenderOption::R_WireFrame)
	{
		values->RenderOption->TurnOnWireframeMode();
	}

	partOption = meshRenderOption & 240;
	if (partOption == (unsigned int)MeshRenderOption::D_DepthOff)
	{
		values->RenderOption->TurnOffZBuffer();
	}

	partOption = meshRenderOption & 3840;
	if (partOption == (unsigned int)MeshRenderOption::B_BlendOn)
	{
		values->RenderOption->TurnOnAlphaBlending();
	}
	else if (partOption == (unsigned int)MeshRenderOption::B_BlendOn2)
	{
		values->RenderOption->TurnOnOtherBlending();
	}
}

template<typename T>
inline void MeshData<T>::OffOption(GlobalValues* values)
{
	if (meshRenderOption == 0)
		return;

	values->RenderOption->TurnOnCullMode();
	values->RenderOption->TurnOnZBuffer();
	values->RenderOption->TurnOffAlphaBlending();
}


template<>
inline void MeshData<Vertex>::CopyBinaryMeshFile(wstring path)
{
	Mesh mesh;
	VertexTexture* tempV;
	UINT* tempI;

	mesh.Open(path);
	mesh.CopyVertex(&tempV, &vertexCount);
	mesh.CopyIndex(&tempI, &indexCount);

	vertexData.resize(vertexCount);
	indexData.resize(indexCount);

	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].position = tempV[i].position;
	}
	for (UINT i = 0; i < indexCount; i++)
	{
		indexData[i] = tempI[i];
	}

	SAFE_DELETE_ARRAY(tempV);
	SAFE_DELETE_ARRAY(tempI);
}


template<>
inline void MeshData<VertexTextureNormalTangentBinormal>::CopyBinaryMeshFile(wstring path)
{
	Mesh mesh;
	VertexTexture* tempV;
	UINT* tempI;

	mesh.Open(path);
	mesh.CopyVertex(&tempV, &vertexCount);
	mesh.CopyIndex(&tempI, &indexCount);

	vertexData.resize(vertexCount);
	indexData.resize(indexCount);

	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].position = tempV[i].position;
		vertexData[i].uv = tempV[i].uv;
	}
	for (UINT i = 0; i < indexCount; i++)
	{
		indexData[i] = tempI[i];
	}
	CalcNormal();
	CalcTangentBinormal();

	SAFE_DELETE_ARRAY(tempV);
	SAFE_DELETE_ARRAY(tempI);
}

template<typename T>
inline void MeshData<T>::ReadBinaryMeshFile(wstring fileName)
{
	Mesh mesh;
	VertexTexture* tempV;
	UINT* tempI;

	mesh.Open(fileName);
	mesh.CopyVertex(&tempV, &vertexCount);
	mesh.CopyIndex(&tempI, &indexCount);

	vertexData.resize(vertexCount);
	indexData.resize(indexCount);

	CopyBinaryMeshFile();

	SAFE_DELETE_ARRAY(tempV);
	SAFE_DELETE_ARRAY(tempI);
}

template<typename T>
inline void MeshData<T>::ReadBinaryFBXFile(wstring fileName)
{
}

template<typename T>
inline void MeshData<T>::ReadBinaryCustumFile(wstring fileName)
{
	BinaryReader* r = new BinaryReader();
	r->Open(fileName);

	vertexCount = r->UInt();
	vertexData.resize(vertexCount);

	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].position = r->Vector3();

		//vertexData[i].uv = r->Vector2();

		//vertexData[i].normal = r->Vector3();
	}

	indexCount = r->UInt();
	indexData.resize(indexCount);

	for (UINT i = 0; i < indexCount; i++)
	{
		indexData[i] = r->UInt();
	}

	r->Close();
}
template<>
inline void MeshData<VertexTextureNormalTangentBinormal>::ReadBinaryCustumFile(wstring fileName)
{
	BinaryReader* r = new BinaryReader();
	r->Open(fileName);

	vertexCount = r->UInt();
	vertexData.resize(vertexCount);

	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].position = r->Vector3();

		vertexData[i].uv = r->Vector2();

		vertexData[i].normal = r->Vector3();
	}

	indexCount = r->UInt();
	indexData.resize(indexCount);

	for (UINT i = 0; i < indexCount; i++)
	{
		indexData[i] = r->UInt();
	}

	CalcTangentBinormal();

	r->Close();
}

template<>
inline void MeshData<VertexTexture>::ReadBinaryCustumFile(wstring fileName)
{
	BinaryReader* r = new BinaryReader();
	r->Open(fileName);

	vertexCount = r->UInt();
	vertexData.resize(vertexCount);

	for (UINT i = 0; i < vertexCount; i++)
	{
		vertexData[i].position = r->Vector3();

		vertexData[i].uv = r->Vector2();
	}

	indexCount = r->UInt();
	indexData.resize(indexCount);

	for (UINT i = 0; i < indexCount; i++)
	{
		indexData[i] = r->UInt();
	}

	r->Close();
}
template<typename T>
inline void MeshData<T>::ReadFBXFile(wstring fileName)
{
}


