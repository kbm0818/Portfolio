#include "../stdafx.h"
#include "MoAnimationKeyFrames.h"
#include "../Model/ModelAnimationKeyFrames.h"

MoAnimationKeyFrames::MoAnimationKeyFrames(wstring animationName)
	: animationName(animationName)
{
	
}


MoAnimationKeyFrames::~MoAnimationKeyFrames()
{

}

void MoAnimationKeyFrames::AddKeyFrame(const D3DXMATRIX & transform)
{
	keyFrames.push_back(make_pair(transform, MoAnimationQuaternionKeyFrame(transform)));
}

wstring MoAnimationKeyFrames::GetAnimationName()
{
	return animationName;
}

const D3DXMATRIX & MoAnimationKeyFrames::GetKeyFrameTransform(int keyFrame) const
{
	return keyFrames[keyFrame].first;
}

const MoAnimationQuaternionKeyFrame & MoAnimationKeyFrames::GetQuaternionKeyFrame(int keyFrame) const
{
	return keyFrames[keyFrame].second;
}

void MoAnimationKeyFrames::Write(BinaryWriter * w)
{
	w->WString(animationName);

	w->UInt(keyFrames.size());
	for (size_t i = 0; i < keyFrames.size(); i++)
	{
		w->Matrix(keyFrames[i].first);
		keyFrames[i].second.Write(w);
	}
}

void MoAnimationKeyFrames::Write(BinaryWriter * w, ModelAnimationKeyFrames * qf)
{
	w->WString(qf->animationName);

	w->UInt(qf->keyFrames.size());
	for (size_t i = 0; i < qf->keyFrames.size(); i++)
	{
		w->Matrix(qf->keyFrames[i].first);
		MoAnimationQuaternionKeyFrame::Write(w, &(qf->keyFrames[i].second));
	}
}

void MoAnimationKeyFrames::Read(BinaryReader * r, ModelAnimationKeyFrames * qf)
{
	qf->animationName = r->Wstring();
	UINT size = r->UInt();

	qf->keyFrames.resize(size);
	for (UINT i = 0; i < size; i++)
	{
		qf->keyFrames[i].first = r->Matrix();
		MoAnimationQuaternionKeyFrame::Read(r, &(qf->keyFrames[i].second));
	}
}

void MoAnimationQuaternionKeyFrame::Write(BinaryWriter * w)
{
	w->QUATERNION(q);
	w->Vector3(translation);
}

void MoAnimationQuaternionKeyFrame::Write(BinaryWriter * w, ModelAnimationQuaternionKeyFrame * qf)
{
	w->QUATERNION(qf->q);
	w->Vector3(qf->translation);
}

void MoAnimationQuaternionKeyFrame::Read(BinaryReader * r, ModelAnimationQuaternionKeyFrame * qf)
{
	qf->q = r->QUATERNION();
	qf->translation = r->Vector3();
}
