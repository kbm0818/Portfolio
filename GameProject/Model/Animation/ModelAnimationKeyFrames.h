#pragma once

class ModelAnimationQuaternionKeyFrame
{
public:
	//매트릭스를 쿼터니언 값으로 변환한다.
	ModelAnimationQuaternionKeyFrame(const D3DXMATRIX& transform)
	{
		D3DXQuaternionRotationMatrix(&q, &transform);

		translation = D3DXVECTOR3(transform._41, transform._42, transform._43);
	}
	ModelAnimationQuaternionKeyFrame(D3DXVECTOR4 q, D3DXVECTOR3 t)
		:q(q), translation(t)
	{

	}
	ModelAnimationQuaternionKeyFrame(const ModelAnimationQuaternionKeyFrame& copy);

	const D3DXQUATERNION& GetQuaternion() const { return q; }
	const D3DXVECTOR3& GetTranslation() const { return translation; }
	void SetData(D3DXVECTOR3 t, D3DXQUATERNION q) { this->q = q; this->translation = t; }
	void SetData(D3DXMATRIX& transform)
	{
		D3DXQuaternionRotationMatrix(&q, &transform);
		translation = D3DXVECTOR3(transform._41, transform._42, transform._43);
	}
	void GetMatrix(D3DXMATRIX* transform)
	{
		D3DXMatrixRotationQuaternion(transform, &q);
		transform->_41 = translation.x;
		transform->_42 = translation.y;
		transform->_43 = translation.z;
	}

private:
	D3DXQUATERNION q;
	D3DXVECTOR3 translation;
};

class ModelAnimationKeyFrames
{
public:
	ModelAnimationKeyFrames(wstring animationName);
	~ModelAnimationKeyFrames();
	ModelAnimationKeyFrames(ModelAnimationKeyFrames& copy);

	void AddKeyFrame(const D3DXMATRIX& transform);

	wstring GetAnimationName();
	void SetAnimationName(wstring name) { animationName = name; }
	const D3DXMATRIX& GetKeyFrameTransform(int keyFrame) const;
	const ModelAnimationQuaternionKeyFrame& GetQuaternionKeyFrame(int keyFrame) const;

	size_t GetKeyFrameCount() { return keyFrames.size(); }

private:
	friend class ModelLoader;

	wstring animationName;

	typedef pair<D3DXMATRIX, ModelAnimationQuaternionKeyFrame> Pair;
	vector<Pair> keyFrames;

	void Write(BinaryWriter* w);
	static void Read(BinaryReader* r, ModelAnimationKeyFrames* dest);
	static void Write(BinaryWriter* w, ModelAnimationKeyFrames* dest);
};

