#include "../stdafx.h"
#include "ConverterBoneWeights.h"

const UINT ConverterBoneWeights::MaxBonesPerVertex = 4;
const UINT ConverterBoneWeights::MaxBonesPerMax = 100;

ConverterBoneWeights::ConverterBoneWeights()
	: sumWeight(0.0f)
{

}

ConverterBoneWeights::ConverterBoneWeights(const ConverterBoneWeights & otherConverterBoneWeights)
{
	this->sumWeight = otherConverterBoneWeights.sumWeight;

	boneWeights.clear();
	boneWeights.assign(otherConverterBoneWeights.boneWeights.begin(), otherConverterBoneWeights.boneWeights.end());
}

ConverterBoneWeights::~ConverterBoneWeights()
{
}

void ConverterBoneWeights::AddBoneWeight(int boneIndex, float boneWeight)
{
	if (boneWeight <= 0.0f)
		return;

	sumWeight += boneWeight;

	// �ߺ�üũ
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

void ConverterBoneWeights::AddBoneWeight(std::pair<int, float> boneWeightPair)
{
	AddBoneWeight(boneWeightPair.first, boneWeightPair.second);
}

void ConverterBoneWeights::AddBoneWeights(const ConverterBoneWeights & boneWeights)
{
	for (int i = 0; i < boneWeights.GetBoneWeightCount(); ++i)
	{
		AddBoneWeight(boneWeights.GetBoneWeight(i));
	}
}

/********************************************************************************
 @brief
 Vertex�� ������ �ִ� Bone�� ���� �ִ�ġ�� ���� �ʵ��� ����
********************************************************************************/
void ConverterBoneWeights::Validate()
{
	sumWeight = 0.0f;
	int i = 0;

	vector<pair<int, float>>::iterator it = boneWeights.begin();
	while (it != boneWeights.end())
	{
		// Bone Weight�� �ִ�ġ�� �Ѿ�� �ִ�ġ �̻��� BoneWeight ����
		if (i >= MaxBonesPerVertex)
		{
			it = boneWeights.erase(it);
		}
		else
		{
			// �ִ�ġ ���� Weight���鸸 �ջ�
			sumWeight += it->second;

			++i; ++it;
		}
	}
}

/********************************************************************************
 @brief
 �� BoneWeight���� �� BoneWeight�� �տ� ���� ������ ��ȯ
 ��� BoneWeight�� ���� ���� 1�� �ǵ��� ����
********************************************************************************/
void ConverterBoneWeights::Normalize()
{
	Validate(); //Bone Weight�� ���� ��ȿ�ϵ��� ����
	
	float fScale = 1.0f / sumWeight;
	std::vector<std::pair<int, float>>::iterator it = boneWeights.begin();
	
	while (it != boneWeights.end())
	{
		it->second *= fScale;

		++it;
	}
}

/********************************************************************************
 @brief
 ��� Bone Weight ������ �ϳ��� Blend Weights Class �����Ͽ� ��ȯ
********************************************************************************/
ConverterBlendWeights ConverterBoneWeights::GetBlendWeights()
{
	ConverterBlendWeights blendWeights;

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