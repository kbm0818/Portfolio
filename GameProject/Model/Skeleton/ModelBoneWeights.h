#pragma once

//해당 정점이 Bone들에 대해서 어느정도의 영향을 받는지
struct ModelBlendWeights;

class ModelBoneWeights
{
public:
	ModelBoneWeights();
	~ModelBoneWeights();

	static const UINT MaxBonesPerVertex;
	static const UINT MaxBonesPerMax;

	void AddBoneWeight(int boneIndex, float boneWeight);
	void AddBoneWeight(pair<int, float> boneWeightPair);
	void AddBoneWeights(const ModelBoneWeights& boneWeights);

	void Validate();
	void Normalize();

	int GetBoneWeightCount() const { return boneWeights.size(); }
	pair<int, float> GetBoneWeight(int index) const { return boneWeights[index]; }
	ModelBlendWeights GetBlendWeights();

private:
	//가중치 총합
	float sumWeight;

	//실질적인 가중치 정보들. first: boneIndex, second: weight
	vector<pair<int, float>> boneWeights;
};


//보통 1개의 정점의 최대 4개까지의 Bone에게 영향을 받는다.
//이것을 응용해 D3DXVECTOR4 자료형을 사용해서 각 x,y,z,w값에
//본의 Index와 가중치 정보를 저장해서 ModelBlendWeights라는 구조체로 묶는다.
struct ModelBlendWeights
{
	D3DXVECTOR4 BlendIndices;
	D3DXVECTOR4 BlendWeights;

	ModelBlendWeights()
	{
		BlendIndices = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
		BlendWeights = D3DXVECTOR4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void SetBlendWeight(int nIndex, int nBoneIndex, float fWeight)
	{
		switch (nIndex)
		{
			case 0:
				BlendIndices.x = (float)nBoneIndex;
				BlendWeights.x = fWeight;
				break;
			case 1:
				BlendIndices.y = (float)nBoneIndex;
				BlendWeights.y = fWeight;
				break;
			case 2:
				BlendIndices.z = (float)nBoneIndex;
				BlendWeights.z = fWeight;
				break;
			case 3:
				BlendIndices.w = (float)nBoneIndex;
				BlendWeights.w = fWeight;
				break;
		}
	}
};