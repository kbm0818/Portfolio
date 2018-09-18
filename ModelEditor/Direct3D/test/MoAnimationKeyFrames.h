#pragma once

class ModelAnimationQuaternionKeyFrame;
class MoAnimationQuaternionKeyFrame
{
public:
	MoAnimationQuaternionKeyFrame(const D3DXMATRIX& transform)
	{
		D3DXQuaternionRotationMatrix(&q, &transform);
		translation = D3DXVECTOR3(transform._41, transform._42, transform._43);
	}

	const D3DXQUATERNION& GetQuaternion() const { return q; }
	const D3DXVECTOR3& GetTranslation() const { return translation; }

	void Write(BinaryWriter * w);
	static void Write(BinaryWriter* w, ModelAnimationQuaternionKeyFrame* qf);
	static void Read(BinaryReader* r, ModelAnimationQuaternionKeyFrame* qf);

private:
	D3DXQUATERNION q;
	D3DXVECTOR3 translation;
};

class ModelAnimationKeyFrames;
class MoAnimationKeyFrames
{
public:
	MoAnimationKeyFrames(wstring animationName);
	~MoAnimationKeyFrames();

	void AddKeyFrame(const D3DXMATRIX& transform);

	wstring GetAnimationName();
	const D3DXMATRIX& GetKeyFrameTransform(int keyFrame) const;
	const MoAnimationQuaternionKeyFrame& GetQuaternionKeyFrame(int keyFrame) const;

	void Write(BinaryWriter * w);
	static void Write(BinaryWriter* w, ModelAnimationKeyFrames* qf);
	static void Read(BinaryReader* r, ModelAnimationKeyFrames* qf);

private:
	wstring animationName;
	UINT keyFramesSize;

	typedef pair<D3DXMATRIX, MoAnimationQuaternionKeyFrame> Pair;
	vector<Pair> keyFrames;
};