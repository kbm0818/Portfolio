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
		//������������ ���ĵǵ��� �Ѵ�.
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

//����Ʈ�� �޾Ƽ� �����سֱ�
void ModelBoneWeights::AddBoneWeights(const ModelBoneWeights & boneWeights)
{
	for (int i = 0; i < boneWeights.GetBoneWeightCount(); ++i)
	{
		AddBoneWeight(boneWeights.GetBoneWeight(i));
	}
}

//��ȿ�� �˻�.
void ModelBoneWeights::Validate()
{
	sumWeight = 0.0f;
	int i = 0;

	//����ġ ������ �޴� ���밡 4���� �Ѿ ��� �������ָ鼭
	//����ġ ������ �ٽ� ����Ѵ�.
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

//����ġ ������ 1.0f���� ����ȭ�����ش�.
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

//BoneWeights Ŭ������ ���� ��� ����ġ ������ D3DXVECTOR4�ڷ��� 2���� ��Ƽ� ��ȯ�Ѵ�.
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
