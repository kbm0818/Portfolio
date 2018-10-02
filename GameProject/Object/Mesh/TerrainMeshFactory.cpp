#include "stdafx.h"
#include "TerrainMeshFactory.h"

#include "Systems\GameSystems\TerrainHeightData.h"
#include "MeshNodeData.h"
#include "MeshData.h"

#define MAX_TRIANGLES 10000
#define HEIGHTDATA exValue->TerrainHeightData

TerrainMeshFactory::TerrainMeshFactory()
{
	data = new MeshNodeData;
}

TerrainMeshFactory::~TerrainMeshFactory()
{
}

MeshNodeData * TerrainMeshFactory::Create(GlobalValues* values)
{
	CreateFullData(values);
	CreateQuadTreeData(values);

	fullVertexData.clear();
	fullIndexData.clear();

	return data;
}

void TerrainMeshFactory::CreateFullData(GlobalValues * exValue)
{
	fWidth = HEIGHTDATA->width;
	fHeight = HEIGHTDATA->height;

	UINT i, j;
	int index, index1, index2, index3, index4;

	// Set the number of vertices in the model.
	fIndexCount = fVertexCount = (fWidth - 1) * (fHeight - 1) * 6;

	// Create the terrain model array.
	fullVertexData.resize(fVertexCount);
	fullIndexData.resize(fIndexCount);

	vector<VertexTextureNormal> &heightData = HEIGHTDATA->heightData;

	// Load the terrain model with the height map terrain data.
	index = 0;

	for (j = 0; j<(fHeight - 1); j++)
	{
		for (i = 0; i<(fWidth - 1); i++)
		{
			index1 = (fWidth * j) + i;          // Bottom left.
			index2 = (fWidth * j) + (i + 1);      // Bottom right.
			index3 = (fWidth * (j + 1)) + i;      // Upper left.
			index4 = (fWidth * (j + 1)) + (i + 1);  // Upper right.

													// Upper left.
			fullVertexData[index].position = heightData[index3].position;
			fullVertexData[index].normal = heightData[index3].normal;
			fullVertexData[index].uv = D3DXVECTOR2(0.0f,0.0f);
			fullIndexData[index] = index;
			index++;

			// Upper right.
			fullVertexData[index].position = heightData[index4].position;
			fullVertexData[index].normal = heightData[index4].normal;
			fullVertexData[index].uv = D3DXVECTOR2(0.0f, 1.0f);
			fullIndexData[index] = index;
			index++;

			// Bottom left.
			fullVertexData[index].position = heightData[index1].position;
			fullVertexData[index].normal = heightData[index1].normal;
			fullVertexData[index].uv = D3DXVECTOR2(1.0f, 0.0f);
			fullIndexData[index] = index;
			index++;

			// Bottom left.
			fullVertexData[index].position = heightData[index1].position;
			fullVertexData[index].normal = heightData[index1].normal;
			fullVertexData[index].uv = D3DXVECTOR2(1.0f, 0.0f);
			fullIndexData[index] = index;
			index++;

			// Upper right.
			fullVertexData[index].position = heightData[index4].position;
			fullVertexData[index].normal = heightData[index4].normal;
			fullVertexData[index].uv = D3DXVECTOR2(0.0f, 1.0f);
			fullIndexData[index] = index;
			index++;

			// Bottom right.
			fullVertexData[index].position = heightData[index2].position;
			fullVertexData[index].normal = heightData[index2].normal;
			fullVertexData[index].uv = D3DXVECTOR2(1.0f, 1.0f);
			fullIndexData[index] = index;
			index++;
		}
	}

	for (UINT i = 0; i < fIndexCount;)
	{
		VertexType &vertex1 = fullVertexData[fullIndexData[i++]];
		VertexType &vertex2 = fullVertexData[fullIndexData[i++]];
		VertexType &vertex3 = fullVertexData[fullIndexData[i++]];

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

	for (UINT i = 0; i < fIndexCount; i++)
	{
		VertexType &V = fullVertexData[fullIndexData[i]];

		D3DXVec3Normalize(&V.tangent, &V.tangent);
		D3DXVec3Normalize(&V.binormal, &V.binormal);
	}
}

void TerrainMeshFactory::CreateQuadTreeData(GlobalValues * exValue)
{
	float centerX, centerZ, width;

	// Store the total triangle count for the vertex list.
	triangleCount = fVertexCount / 3;

	CalculateMeshDimensions(fVertexCount, centerX, centerZ, width);

	// Create the parent node for the quad tree.
	data->node = new MeshNode;
	assert(data->node);

	// Recursively build the quad tree based on the vertex list data and mesh dimensions.
	CreateTreeNode(data->node, centerX, centerZ, width);
}

void TerrainMeshFactory::CalculateMeshDimensions(int vertexCount, float & centerX, float & centerZ, float & meshWidth)
{
	int i;
	float maxWidth, maxDepth, minWidth, minDepth, width, depth, maxX, maxZ;


	// Initialize the center position of the mesh to zero.
	centerX = 0.0f;
	centerZ = 0.0f;

	// Sum all the vertices in the mesh.
	for (i = 0; i<vertexCount; i++)
	{
		centerX += fullVertexData[i].position.x;
		centerZ += fullVertexData[i].position.z;
	}

	// And then divide it by the number of vertices to find the mid-point of the mesh.
	centerX = centerX / (float)vertexCount;
	centerZ = centerZ / (float)vertexCount;

	// Initialize the maximum and minimum size of the mesh.
	maxWidth = 0.0f;
	maxDepth = 0.0f;

	minWidth = fabsf(fullVertexData[0].position.x - centerX);
	minDepth = fabsf(fullVertexData[0].position.z - centerZ);

	// Go through all the vertices and find the maximum and minimum width and depth of the mesh.
	for (i = 0; i<vertexCount; i++)
	{
		width = fabsf(fullVertexData[i].position.x - centerX);
		depth = fabsf(fullVertexData[i].position.z - centerZ);

		if (width > maxWidth) { maxWidth = width; }
		if (depth > maxDepth) { maxDepth = depth; }
		if (width < minWidth) { minWidth = width; }
		if (depth < minDepth) { minDepth = depth; }
	}

	// Find the absolute maximum value between the min and max depth and width.
	maxX = (float)max(fabs(minWidth), fabs(maxWidth));
	maxZ = (float)max(fabs(minDepth), fabs(maxDepth));

	// Calculate the maximum diameter of the mesh.
	meshWidth = max(maxX, maxZ) * 2.0f;
}

void TerrainMeshFactory::CreateTreeNode(MeshNode * node, float positionX, float positionZ, float width)
{
	int numTriangles, i, count, vertexCount, index, vertexIndex;
	float offsetX, offsetZ;
	VertexType* vertices;
	unsigned long* indices;
	bool result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexDataSRS, indexDataSRS;

	// Store the node position and size.
	node->positionX = positionX;
	node->positionZ = positionZ;
	node->width = width;

	// Initialize the triangle count to zero for the node.
	node->triangleCount = 0;

	// Initialize the mesh
	node->mesh = new MeshData<VertexType>;

	// Initialize the children nodes of this node to null.
	node->nodes[0] = 0;
	node->nodes[1] = 0;
	node->nodes[2] = 0;
	node->nodes[3] = 0;

	// Count the number of triangles that are inside this node.
	numTriangles = CountTriangles(positionX, positionZ, width);

	// Case 1: If there are no triangles in this node then return as it is empty and requires no processing.
	if (numTriangles == 0)
	{
		return;
	}

	// Case 2: If there are too many triangles in this node then split it into four equal sized smaller tree nodes.
	if (numTriangles > MAX_TRIANGLES)
	{
		for (i = 0; i<4; i++)
		{
			// Calculate the position offsets for the new child node.
			offsetX = (((i % 2) < 1) ? -1.0f : 1.0f) * (width / 4.0f);
			offsetZ = (((i % 4) < 2) ? -1.0f : 1.0f) * (width / 4.0f);

			// See if there are any triangles in the new node.
			count = CountTriangles((positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
			if (count > 0)
			{
				// If there are triangles inside where this new node would be then create the child node.
				node->nodes[i] = new MeshNode;

				// Extend the tree starting from this new child node now.
				CreateTreeNode(node->nodes[i], (positionX + offsetX), (positionZ + offsetZ), (width / 2.0f));
			}
		}

		return;
	}

	// Case 3: If this node is not empty and the triangle count for it is less than the max then 
	// this node is at the bottom of the tree so create the list of triangles to store in it.
	node->triangleCount = numTriangles;

	// Calculate the number of vertices.
	vertexCount = numTriangles * 3;

	node->mesh->vertexCount = vertexCount;
	node->mesh->indexCount = vertexCount;

	// Create the vertex array.
	vertices = new VertexType[vertexCount];

	// Create the index array.
	indices = new unsigned long[vertexCount];

	// Initialize the index for this new vertex and index array.
	index = 0;

	// Go through all the triangles in the vertex list.
	for (i = 0; i<triangleCount; i++)
	{
		// If the triangle is inside this node then add it to the vertex array.
		result = IsTriangleContained(i, positionX, positionZ, width);
		if (result == true)
		{
			// Calculate the index into the terrain vertex list.
			vertexIndex = i * 3;

			// Get the three vertices of this triangle from the vertex list.
			vertices[index].position = fullVertexData[vertexIndex].position;
			vertices[index].uv = fullVertexData[vertexIndex].uv;
			vertices[index].normal = fullVertexData[vertexIndex].normal;
			vertices[index].tangent = fullVertexData[vertexIndex].tangent;
			vertices[index].binormal = fullVertexData[vertexIndex].binormal;

			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index].position = fullVertexData[vertexIndex].position;
			vertices[index].uv = fullVertexData[vertexIndex].uv;
			vertices[index].normal = fullVertexData[vertexIndex].normal;
			vertices[index].tangent = fullVertexData[vertexIndex].tangent;
			vertices[index].binormal = fullVertexData[vertexIndex].binormal;

			indices[index] = index;
			index++;

			vertexIndex++;
			vertices[index].position = fullVertexData[vertexIndex].position;
			vertices[index].uv = fullVertexData[vertexIndex].uv;
			vertices[index].normal = fullVertexData[vertexIndex].normal;
			vertices[index].tangent = fullVertexData[vertexIndex].tangent;
			vertices[index].binormal = fullVertexData[vertexIndex].binormal;

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
	vertexDataSRS.pSysMem = vertices;
	vertexDataSRS.SysMemPitch = 0;
	vertexDataSRS.SysMemSlicePitch = 0;

	// Now finally create the vertex buffer.
	D3D::GetDevice()->CreateBuffer(&vertexBufferDesc, &vertexDataSRS, &node->mesh->vertexBuffer);

	// Set up the description of the index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * vertexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexDataSRS.pSysMem = indices;
	indexDataSRS.SysMemPitch = 0;
	indexDataSRS.SysMemSlicePitch = 0;

	// Create the index buffer.
	D3D::GetDevice()->CreateBuffer(&indexBufferDesc, &indexDataSRS, &node->mesh->indexBuffer);

	// Release the vertex and index arrays now that the data is stored in the buffers in the node.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;
}

int TerrainMeshFactory::CountTriangles(float positionX, float positionZ, float width)
{
	int count, i;
	bool result;


	// Initialize the count to zero.
	count = 0;

	// Go through all the triangles in the entire mesh and check which ones should be inside this node.
	for (i = 0; i< triangleCount; i++)
	{
		// If the triangle is inside the node then increment the count by one.
		result = IsTriangleContained(i, positionX, positionZ, width);
		if (result == true)
		{
			count++;
		}
	}

	return count;
}

bool TerrainMeshFactory::IsTriangleContained(int index, float positionX, float positionZ, float width)
{
	float radius;
	int vertexIndex;
	float x1, z1, x2, z2, x3, z3;
	float minimumX, maximumX, minimumZ, maximumZ;


	// Calculate the radius of this node.
	radius = width / 2.0f;

	// Get the index into the vertex list.
	vertexIndex = index * 3;

	// Get the three vertices of this triangle from the vertex list.
	x1 = fullVertexData[vertexIndex].position.x;
	z1 = fullVertexData[vertexIndex].position.z;
	vertexIndex++;

	x2 = fullVertexData[vertexIndex].position.x;
	z2 = fullVertexData[vertexIndex].position.z;
	vertexIndex++;

	x3 = fullVertexData[vertexIndex].position.x;
	z3 = fullVertexData[vertexIndex].position.z;

	// Check to see if the minimum of the x coordinates of the triangle is inside the node.
	minimumX = min(x1, min(x2, x3));
	if (minimumX > (positionX + radius))
	{
		return false;
	}

	// Check to see if the maximum of the x coordinates of the triangle is inside the node.
	maximumX = max(x1, max(x2, x3));
	if (maximumX < (positionX - radius))
	{
		return false;
	}

	// Check to see if the minimum of the z coordinates of the triangle is inside the node.
	minimumZ = min(z1, min(z2, z3));
	if (minimumZ >(positionZ + radius))
	{
		return false;
	}

	// Check to see if the maximum of the z coordinates of the triangle is inside the node.
	maximumZ = max(z1, max(z2, z3));
	if (maximumZ < (positionZ - radius))
	{
		return false;
	}

	return true;
}
