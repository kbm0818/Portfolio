#pragma once

class Capsule2D
{
public:
	Capsule2D(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	Capsule2D(D3DXVECTOR3 pos, D3DXQUATERNION rot);
	~Capsule2D();

	void Update();
	void Render();

	void SetPosition(D3DXVECTOR3 pos) { position = pos; }
	void SetRotation(D3DXVECTOR3 rot)
	{
		D3DXQuaternionRotationYawPitchRoll(&rotation,
			rot.y, rot.x, rot.z);
	}
	void SetWorld(D3DXMATRIX& mat)
	{
		memcpy(&world, &mat, sizeof(D3DXMATRIX));
	}
	D3DXMATRIX& GetWorld() { return world; }

	void UpdateVertex(float radius, float height);

private:
	typedef VertexColor VertexType;

	void CreateBuffer();

	Shader* shader;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	UINT vertexCount;
	UINT indexCount;

	D3DXMATRIX world;
	D3DXVECTOR3 position;
	D3DXQUATERNION rotation;

	WorldBuffer* worldBuffer;
};