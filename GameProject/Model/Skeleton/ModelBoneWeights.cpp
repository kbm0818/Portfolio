#include "stdafx.h"
#include "./ModelBoneWeights.h"

const UINT ModelBoneWeights::MaxBonesPerVertex = 4;
const UINT ModelBoneWeights::MaxBonesPerMax = 100;

ModelBoneWeights::ModelBoneWeights()
	: sumWeight(0.0f)
{
}

ModelBoneWeights::~ModelBoneWeights()
{
}

void ModelBoneWeights::AddBoneWeight(int boneIndex, float boneWeight)
{
	if (boneWeight <= 0.0f)
		return;

	sumWeight += boneWeight;

	bool isAdded = false;
	std::vector<std::pair<int, float>>::iterator it;
	for (it = boneWeights.begin(); it != boneWeights.end(); it++)
	{
		//오름차순으로 정렬되도록 한다.
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

void ModelBoneWeights::AddBoneWeight(pair<int, float> boneWeightPair)
{
	AddBoneWeight(boneWeightPair.first, boneWeightPair.second);
}

//리스트를 받아서 복사해넣기
void ModelBoneWeights::AddBoneWeights(const ModelBoneWeights & boneWeights)
{
	for (int i = 0; i < boneWeights.GetBoneWeightCount(); ++i)
	{
		AddBoneWeight(boneWeights.GetBoneWeight(i));
	}
}

//유효성 검사.
void ModelBoneWeights::Validate()
{
	sumWeight = 0.0f;
	int i = 0;

	//가중치 영향을 받는 뼈대가 4개를 넘어갈 경우 삭제해주면서
	//가중치 총합을 다시 계산한다.
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

//가중치 총합을 1.0f으로 정규화시켜준다.
void ModelBoneWeights::Normalize()
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

//BoneWeights 클래스가 가진 모든 가중치 정보를 D3DXVECTOR4자료형 2개에 담아서 반환한다.
ModelBlendWeights ModelBoneWeights::GetBlendWeights()
{
	ModelBlendWeights blendWeights;

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
