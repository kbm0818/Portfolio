#include "stdafx.h"
#include "TerrainHeightData.h"

#define TEXTURE_REPEAT 8

TerrainHeightData::TerrainHeightData()
	: width(0), height(0)
{
}

TerrainHeightData::~TerrainHeightData()
{
}

void TerrainHeightData::Init(wstring path, HeightMapFileType type, float scale, float divideValue)
{
	this->scale = scale;

	switch (type)
	{
	case HeightMapFileType::Texture:
		LoadHeightMap(path, scale, divideValue);
		CalcNormal();
		CalcUv();
		break;
	case HeightMapFileType::binary:
		LoadBinary(path);
		break;
	}
	int a = 10;
}

float TerrainHeightData::GetY(D3DXVECTOR3 & position)
{
	float x = floor(position.x);
	float z = floor(position.z);

	if ((x < 0 || x >= width) || (z < 0 || z >= height))
	{
		return 0.0f;
	}

	UINT index0 = width * (UINT)z + (UINT)x;
	UINT index1 = width * ((UINT)z + 1) + (UINT)x;
	UINT index2 = width * (UINT)z + (UINT)x + 1;
	UINT index3 = width * ((UINT)z + 1) + (UINT)x + 1;

	D3DXVECTOR3 v0 = heightData[index0].position;
	D3DXVECTOR3 v1 = heightData[index1].position;
	D3DXVECTOR3 v2 = heightData[index2].position;
	D3DXVECTOR3 v3 = heightData[index3].position;

	float deltaX = (position.x) - v0.x;
	float deltaZ = (position.z) - v0.z;

	D3DXVECTOR3 temp;
	if (deltaX + deltaZ <= 1)
	{
		temp = v0 + (v2 - v0) * deltaX + (v1 - v0) * deltaZ;
	}
	else
	{
		deltaX = 1 - deltaX;
		deltaZ = 1 - deltaZ;

		temp = v3 + (v1 - v3) * deltaX + (v2 - v3)*deltaZ;
	}

	return temp.y;
}

void TerrainHeightData::SaveBinary(wstring path)
{
	if (heightData.empty())
		return;

	BinaryWriter* w = new BinaryWriter();
	w->Open(path);
	w->UInt(width);
	w->UInt(height);
	for (UINT j = 0; j < height; j++)
	{
		for (UINT i = 0; i < width; i++)
		{
			w->Vector3(heightData[(height * j) + i].position);
			w->Vector2(heightData[(height * j) + i].uv);
			w->Vector3(heightData[(height * j) + i].normal);
		}
	}
	w->Close();
	SAFE_DELETE(w);

}

void TerrainHeightData::LoadBinary(wstring path)
{
	BinaryReader* r = new BinaryReader();
	r->Open(path);
	width = r->UInt();
	height = r->UInt();

	heightData.resize(width * height);
	for (UINT j = 0; j < height; j++)
	{
		for (UINT i = 0; i < width; i++)
		{
			heightData[(height * j) + i].position = r->Vector3();
			heightData[(height * j) + i].uv = r->Vector2();
			heightData[(height * j) + i].normal = r->Vector3();
		}
	}
	r->Close();
	SAFE_DELETE(r);
}

void TerrainHeightData::LoadHeightMap(wstring path, float scale, float divideValue)
{
	FILE* filePtr;
	int error;
	unsigned int count;
	BITMAPFILEHEADER bitmapFileHeader;
	BITMAPINFOHEADER bitmapInfoHeader;
	int imageSize, index;
	UINT i, j, k;
	unsigned char* bitmapImage;

	// Open the height map file in binary.
	error = fopen_s(&filePtr, String::WStringToString(path).c_str(), "rb");
	assert(error == 0);

	// Read in the file header.
	count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	assert(count == 1);

	// Read in the bitmap info header.
	count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
	assert(count == 1);

	// Save the dimensions of the terrain.
	width = bitmapInfoHeader.biWidth;
	height = bitmapInfoHeader.biHeight;

	// Calculate the size of the bitmap image data.
	imageSize = width * height * 3;

	// Allocate memory for the bitmap image data.
	bitmapImage = new unsigned char[imageSize];
	assert(bitmapImage);

	// Move to the beginning of the bitmap data.
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// Read in the bitmap image data.
	count = fread(bitmapImage, 1, imageSize, filePtr);
	assert(count == imageSize);

	// Close the file.
	error = fclose(filePtr);
	assert(error == 0);

	// Create the structure to hold the height map data.
	heightData.resize(width * height);

	// Initialize the position in the image data buffer.
	k = 0;

	// Read the image data into the height map.
	for (j = 0; j < height; j++)
	{
		for (i = 0; i< width; i++)
		{
			index = (height * j) + i;

			heightData[index].position.x = (float)i * scale;
			//높이를 완만하게 줄이기위해 15.0f로 나눠주었음
			//TODO:: 높이맵 높이 임의로 줄임
			heightData[index].position.y = (float)bitmapImage[k] / divideValue;
			heightData[index].position.z = (float)j * scale;

			k += 3;
		}
	}

	// Release the bitmap image data.
	SAFE_DELETE_ARRAY(bitmapImage);
}

void TerrainHeightData::CalcNormal()
{
	int index1, index2, index3, index, count;
	D3DXVECTOR3 vertex1, vertex2, vertex3, vector1, vector2, sum;
	float length;
	vector<D3DXVECTOR3> normals;


	// Create a temporary array to hold the un-normalized normal vectors.
	normals.resize((height - 1) * (width - 1));

	// Go through all the faces in the mesh and calculate their normals.
	for (int j = 0; j<((int)height - 1); j++)
	{
		for (int i = 0; i<((int)width - 1); i++)
		{
			index1 = (j * width) + i;
			index2 = (j * width) + (i + 1);
			index3 = ((j + 1) * width) + i;

			// Get three vertices from the face.
			vertex1 = heightData[index1].position;
			vertex2 = heightData[index2].position;
			vertex3 = heightData[index3].position;

			// Calculate the two vectors for this face.
			vector1 = vertex1 - vertex3;
			vector2 = vertex3 - vertex2;

			index = (j * (width - 1)) + i;

			// Calculate the cross product of those two vectors to get the un-normalized value for this face normal.
			normals[index].x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
			normals[index].y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
			normals[index].z = (vector1.x * vector2.y) - (vector1.y * vector2.x);
		}
	}

	// Now go through all the vertices and take an average of each face normal that the vertex touches to get the averaged normal for that vertex.
	for (int j = 0; j< (int)height; j++)
	{
		for (int i = 0; i< (int)width; i++)
		{
			// Initialize the sum.
			sum = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			// Initialize the count.
			count = 0;

			// Bottom left face.
			bool b1 = (i - 1) >= 0;
			bool b2 = (j - 1) >= 0;
			b1 = b1 && b2;

			if (((i - 1) >= 0) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (width - 1)) + (i - 1);

				sum += normals[index];

				count++;
			}

			// Bottom right face.
			if ((i < ((int)width - 1)) && ((j - 1) >= 0))
			{
				index = ((j - 1) * (height - 1)) + i;

				sum += normals[index];

				count++;
			}

			// Upper left face.
			if (((i - 1) >= 0) && (j < ((int)height - 1)))
			{
				index = (j * (height - 1)) + (i - 1);

				sum += normals[index];

				count++;
			}

			// Upper right face.
			if ((i < ((int)width - 1)) && (j < ((int)height - 1)))
			{
				index = (j * (height - 1)) + i;

				sum += normals[index];

				count++;
			}

			// Take the average of the faces touching this vertex.
			sum /= (float)count;

			// Calculate the length of this normal.
			length = D3DXVec3Length(&sum);

			// Get an index to the vertex location in the height map array.
			index = (j * height) + i;

			// Normalize the final shared normal for this vertex and store it in the height map array.
			heightData[index].normal = sum / length;
		}
	}
}

void TerrainHeightData::CalcUv()
{
	UINT i, j;
	int incrementCount, tuCount, tvCount;
	float incrementValue, tuCoordinate, tvCoordinate;

	// Calculate how much to increment the texture coordinates by.
	incrementValue = (float)TEXTURE_REPEAT / (float)width;

	// Calculate how many times to repeat the texture.
	incrementCount = width / TEXTURE_REPEAT;

	// Initialize the tu and tv coordinate values.
	tuCoordinate = 0.0f;
	tvCoordinate = 1.0f;

	// Initialize the tu and tv coordinate indexes.
	tuCount = 0;
	tvCount = 0;

	// Loop through the entire height map and calculate the tu and tv texture coordinates for each vertex.
	for (j = 0; j< height; j++)
	{
		for (i = 0; i< width; i++)
		{
			// Store the texture coordinate in the height map.
			heightData[(height * j) + i].uv.x = tuCoordinate;
			heightData[(height * j) + i].uv.y = tvCoordinate;

			// Increment the tu texture coordinate by the increment value and increment the index by one.
			tuCoordinate += incrementValue;
			tuCount++;

			// Check if at the far right end of the texture and if so then start at the beginning again.
			if (tuCount == incrementCount)
			{
				tuCoordinate = 0.0f;
				tuCount = 0;
			}
		}

		// Increment the tv texture coordinate by the increment value and increment the index by one.
		tvCoordinate -= incrementValue;
		tvCount++;

		// Check if at the top of the texture and if so then start at the bottom again.
		if (tvCount == incrementCount)
		{
			tvCoordinate = 1.0f;
			tvCount = 0;
		}
	}
}
