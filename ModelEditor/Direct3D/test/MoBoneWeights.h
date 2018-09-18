#pragma once

struct MoBlendWeights
{
	D3DXVECTOR4 BlendIndices;
	D3DXVECTOR4 BlendWeights;

	MoBlendWeights()
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

class MoBoneWeights
{
public:
	MoBoneWeights();
	~MoBoneWeights();

	static const UINT MaxBonesPerVertex;
	static const UINT MaxBonesPerMax;

	void AddBoneWeight(int boneIndex, float boneWeight);
	void AddBoneWeight(pair<int, float> boneWeightPair);
	void AddBoneWeights(const MoBoneWeights& boneWeights);

	void Validate();
	void Normalize();

	int GetBoneWeightCount() const { return boneWeights.size(); }
	pair<int, float> GetBoneWeight(int index) const { return boneWeights[index]; }
	MoBlendWeights GetBlendWeights();

private:
	float sumWeight;

	vector<pair<int, float>> boneWeights;
};