#pragma once
#include "BinaryInputOutputHandler.h"
/********************************************************************************
* @brief
* KeyFrame�� ����� Quaternion ���� Class
*
* Quaternion(ȸ����, ȸ����), Translation(ȸ���� ��ġ)�� ����
********************************************************************************/
class ConverterAnimationQuaternionKeyFrame : public BinaryInputOutputHandler
{
public:
	ConverterAnimationQuaternionKeyFrame() {};
	ConverterAnimationQuaternionKeyFrame(const D3DXMATRIX& transform)
	{
		// Trnasform Matrix�� Quaternion���� ��ȯ
		D3DXQuaternionRotationMatrix(&q, &transform);
		// Translation������ ���� ����
		translation = D3DXVECTOR3(transform._41, transform._42, transform._43);
	}

	const D3DXQUATERNION& GetQuaternion() const { return q; }
	const D3DXVECTOR3& GetTranslation() const { return translation; }

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	D3DXQUATERNION q; /// ȸ����(Vector)�� ȸ������ ���� ������ ���� Quaternion
	D3DXVECTOR3 translation; /// ȸ������ ��ġ(Position) ����
};

/********************************************************************************
 @brief
 AnimationName�� �ش��ϴ� Animation�� ��� KeyFrmae ������ �����ϴ� Class
 
 (KeyFrmae ���� => Transform Matrix�� Quaternion�� Pair�� ����)
********************************************************************************/
class ConverterAnimationKeyFrames : public BinaryInputOutputHandler
{
public:
	ConverterAnimationKeyFrames() {};
	ConverterAnimationKeyFrames(wstring animationName);
	ConverterAnimationKeyFrames(ConverterAnimationKeyFrames& other);
	~ConverterAnimationKeyFrames();
	
	void AddKeyFrame(const D3DXMATRIX& transform);

	wstring GetAnimationName();
	void SetAnimationName(wstring animationName);

	const D3DXMATRIX& GetKeyFrameTransform(int keyFrame) const;
	const ConverterAnimationQuaternionKeyFrame& GetQuaternionKeyFrame(int keyFrame) const;

	virtual void Export(BinaryWriter* bw);
	virtual void Import(BinaryReader* br);

private:
	wstring animationName;

	typedef pair<D3DXMATRIX, ConverterAnimationQuaternionKeyFrame> Pair;
	vector<Pair> keyFrames; ///��� KeyFrame�� ������ �����ϴ� ����
};