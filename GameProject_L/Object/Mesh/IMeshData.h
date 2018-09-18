#pragma once

enum class MeshType
{
	BinaryMeshFile = 0,
	FBXFile,
	BinaryFBXFile,
	BinaryCustumFile,
	UserData,
};

enum MeshRenderOption : unsigned int
{
	R_CullOn = 0,
	R_CullReverce = 2,
	R_CullOff = 4,
	R_WireFrame = 8,

	D_DepthOn = 16,
	D_DepthOff = 32,

	B_BlendOff = 256,
	B_BlendOn = 512,
	B_BlendOn2 = 1024,
};

class IMeshData
{
public:
	virtual ~IMeshData() {};

	//virtual void Initalize(MeshType type, wstring fileName, D3D11_USAGE vertexBufferUsage = D3D11_USAGE_DEFAULT, D3D11_USAGE indexBufferUsage = D3D11_USAGE_DEFAULT) = 0;
	virtual void Update(GlobalValues* values) = 0;
	virtual void Draw(GlobalValues* values, D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) = 0;
	virtual void SetDrawOption(int op) = 0;
};