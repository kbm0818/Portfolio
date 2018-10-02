#include "stdafx.h"
#include "MoBoneWeights.h"

const UINT MoBoneWeights::MaxBonesPerVertex = 4;
const UINT MoBoneWeights::MaxBonesPerMax = 100;

MoBoneWeights::MoBoneWeights()
	: sumWeight(0.0f)
{

}

MoBoneWeights::~MoBoneWeights()
{

}

void MoBoneWeights::AddBoneWeight(int boneIndex, float boneWeight)
{
	if (boneWeight <= 0.0f)
		return;

	sumWeight += boneWeight;

	
	bool isAdded = false;
	std::vector<std::pair<int, float>>::iterator it;
	for (it = boneWeights.begin(); it != boneWeights.end(); it++)
	{
		if (boneWeight > it->second)
		{
			boneWeights.insert(it, std::make_pair(boneIndex, boneWeight));
			isAdded = true;
			break;
		}
	}

	if (isAdded == false)
		boneWeights.push_back(make_pair(boneIndex, boneWeight));
}

void MoBoneWeights::AddBoneWeight(std::pair<int, float> boneWeightPair)
{
	AddBoneWeight(boneWeightPair.first, boneWeightPair.second);
}

void MoBoneWeights::AddBoneWeights(const MoBoneWeights & boneWeights)
{
	for (int i = 0; i < boneWeights.GetBoneWeightCount(); ++i)
	{
		AddBoneWeight(boneWeights.GetBoneWeight(i));
	}
}

void MoBoneWeights::Validate()
{
	sumWeight = 0.0f;
	int i = 0;

	vector<pair<int, float>>::iterator it = boneWeights.begin();
	while (it != boneWeights.end())
	{
		if (i >= MaxBonesPerVertex)
		{
			it = boneWeights.erase(it);
		}
		else
		{
			sumWeight += it->second;

			++i; ++it;
		}
	}
}

void MoBoneWeights::Normalize()
{
	Validate();

	
	float fScale = 1.0f / sumWeight;
	std::vector<std::pair<int, float>>::iterator it = boneWeights.begin();
	
	while (it != boneWeights.end())
	{
		it->second *= fScale;

		++it;
	}
}

MoBlendWeights MoBoneWeights::GetBlendWeights()
{
	MoBlendWeights blendWeights;

	int i = 0;
	vector<pair<int, float>>::iterator it = boneWeights.begin();
	while (it != boneWeights.end())
	{
		blendWeights.SetBlendWeight(i, it->first, it->second);
		++i;
		++it;

		if (i >= MaxBonesPerVertex)
			break;
	}

	return blendWeights;
}

void MoBoneWeights::Write(BinaryWriter * w)
{
	w->Float(sumWeight);

	//w->UInt(boneWeights.size());
	//for (size_t i = 0; i < boneWeights.size(); i++)
	//{
	//	w->Int(boneWeights[i].first);
	//	w->Float(boneWeights[i].second);
	//}

	w->UInt(boneWeights.size());
	w->Byte(&boneWeights[0], sizeof(pair<int, float>) * boneWeights.size());
}

void MoBoneWeights::Read(BinaryReader * r)
{
}

void MoBoneWeights::Write(BinaryWriter * w, ModelBoneWeights * modelBoneWeights)
{
}
