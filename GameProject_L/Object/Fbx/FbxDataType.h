#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////

class FbxAnimationQuaternionKeyFrameData
{
public:
	FbxAnimationQuaternionKeyFrameData(const D3DXMATRIX& transform)
	{
		D3DXQuaternionRotationMatrix(&q, &transform);
		translation = D3DXVECTOR3(transform._41, transform._42, transform._43);
	}

	const D3DXQUATERNION& GetQuaternion() const { return q; }
	const D3DXVECTOR3& GetTranslation() const { return translation; }

	void Write(BinaryWriter * w);

private:
	D3DXQUATERNION q;
	D3DXVECTOR3 translation;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxAnimationKeyFrameDatas
{
public:
	FbxAnimationKeyFrameDatas(wstring animationName);
	~FbxAnimationKeyFrameDatas();

	void AddKeyFrame(const D3DXMATRIX& transform);

	wstring GetAnimationName();
	const D3DXMATRIX& GetKeyFrameTransform(int keyFrame) const;
	const FbxAnimationQuaternionKeyFrameData& GetQuaternionKeyFrame(int keyFrame) const;

	void Write(BinaryWriter * w);

private:
	wstring animationName;
	UINT keyFramesSize;

	typedef pair<D3DXMATRIX, FbxAnimationQuaternionKeyFrameData> Pair;
	vector<Pair> keyFrames;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxAnimationData
{
public:
	FbxAnimationData(wstring name, int keyFrame, float frameRate);
	~FbxAnimationData();

	void Write(BinaryWriter* r);

	wstring GetName() { return name; }
	int GetKeyFrames() { return keyFrame; }

	float GetFrameRate() { return frameRate; }
	void SetFrameRate(float frameRate) { frameRate = frameRate; }

	float GetDefaultFrameRate() { return defaultFrameRate; }

	void ResetFrameRate() { frameRate = defaultFrameRate; }

private:
	wstring name;

	int keyFrame;
	float frameRate;
	float defaultFrameRate;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

enum class FbxAnimationMode
{
	Play = 0, Pause, Stop,
};

class FbxAnimationData;
class FbxAnimationControllerData
{
public:
	FbxAnimationControllerData();
	~FbxAnimationControllerData();

	void AddAnimation(FbxAnimationData* animation);

	FbxAnimationData* GetCurrentAnimation();
	void SetCurrentAnimation(wstring animationName);
	void SetCurrentAnimation(int index);

	UINT GetAnimationCount();
	FbxAnimationData* GetAnimation(int index);
	FbxAnimationData* GetAnimation(wstring animationName);

	void ChangeOldThing();
	void SetDefaultAnim(wstring animationName);
	void UsePopAnimation(wstring animationName);

	void Play();
	void Pause();
	void Stop();

	int GetCurrentKeyFrame() { return currentKeyFrame; }
	void SetCurrentKeyFrame(int keyFrame);

	int GetNextKeyFrame() { return nextKeyFrame; }
	float GetKeyFrameFactor() { return keyFrameFactor; }

	FbxAnimationMode GetAnimationMode() { return animationMode; }

	void UseQuaternionKeyFrames(bool use) { useQuaternionKeyFrames = use; }
	bool UseQuaternionKeyFrames() { return useQuaternionKeyFrames; }

	bool GetUseOld() { return useOld; }

private:
	FbxAnimationMode animationMode;
	FbxAnimationData* currentAnimation;

	typedef pair<wstring, FbxAnimationData *> Pair;
	vector<Pair> animations;

	float frameTimer;

	int currentKeyFrame;
	int nextKeyFrame;
	float keyFrameFactor;

	bool useQuaternionKeyFrames;

	//빽애니 추가
	FbxAnimationData* defaultAnimation;
	bool useOld;

	wstring curAnimName;
};

/////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////

struct FbxBlendWeightsData
{
	D3DXVECTOR4 BlendIndices;
	D3DXVECTOR4 BlendWeights;

	FbxBlendWeightsData()
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

class FbxBoneWeightsData
{
public:
	FbxBoneWeightsData();
	~FbxBoneWeightsData();

	static const UINT MaxBonesPerVertex;
	static const UINT MaxBonesPerMax;

	void AddBoneWeight(int boneIndex, float boneWeight);
	void AddBoneWeight(pair<int, float> boneWeightPair);
	void AddBoneWeights(const FbxBoneWeightsData& boneWeights);

	void Validate();
	void Normalize();

	int GetBoneWeightCount() const { return boneWeights.size(); }
	pair<int, float> GetBoneWeight(int index) const { return boneWeights[index]; }
	FbxBlendWeightsData GetBlendWeights();

	void Write(BinaryWriter* w);

private:
	float sumWeight;

	vector<pair<int, float>> boneWeights;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////


class FbxSkeletonBoneData
{
public:
	FbxSkeletonBoneData(wstring name, int parentBoneIndex);
	~FbxSkeletonBoneData();

	void AddAnimationKeyFrames(FbxAnimationKeyFrameDatas* keyFrames);

	void SetBindPoseTransform(const D3DXMATRIX& matBindPoseTransform);
	void SetBoneReferenceTransform(const D3DXMATRIX& matBoneReferenceTransform);

	FbxAnimationKeyFrameDatas* GetAnimationKeyFrames(wstring animationName);

	void Write(BinaryWriter * w);

	const D3DXMATRIX& GetBindPoseTransform()
	{
		return matBindPoseTransform;
	}

	const D3DXMATRIX& GetInvBindPoseTransform()
	{
		return matInvBindPoseTransform;
	}

	const D3DXMATRIX& GetBoneReferenceTransform()
	{
		return matBoneReferenceTransform;
	}

	const D3DXMATRIX& GetInvBoneReferenceTransform()
	{
		return matInvBoneReferenceTransform;
	}

	wstring GetName() { return name; }
	int GetParentBoneIndex() { return parentBoneIndex; }

private:
	typedef pair<wstring, FbxAnimationKeyFrameDatas *> Pair;

	wstring name;

	D3DXMATRIX matBindPoseTransform;
	D3DXMATRIX matInvBindPoseTransform;
	D3DXMATRIX matBoneReferenceTransform;
	D3DXMATRIX matInvBoneReferenceTransform;

	int parentBoneIndex;

	vector<Pair> animationKeyFrames;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxSkeletonData
{
public:
	FbxSkeletonData();
	~FbxSkeletonData();

	void AddSkeletonBone(FbxSkeletonBoneData* skeletonBone);
	FbxSkeletonBoneData* FindBone(wstring boneName);
	int FindBoneIndex(wstring boneName);

	FbxSkeletonBoneData* GetSkeletonBone(int index);
	const vector<pair<wstring, FbxSkeletonBoneData *>>& GetSkeletonBones()
	{
		return skeletonBones;
	}

	D3DXMATRIX* GetSkinTransforms()
	{
		return skinTransforms;
	}

	D3DXMATRIX GetBoneAnimationTransforms(wstring boneName);


	int GetBoneCount() { return boneCount; }

	void Write(BinaryWriter* w);

private:
	
	typedef pair<wstring, FbxSkeletonBoneData *> Pair;
	vector<Pair> skeletonBones;

	D3DXMATRIX* skinTransforms;
	D3DXMATRIX* boneAnimationTransforms;

	UINT boneCount;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxMaterialData
{
public:
	UINT GetNumber() { return number; }

	FbxMaterialData(int number, FbxSurfaceMaterial* material);
	~FbxMaterialData();

	void Write(BinaryWriter* w);
	

private:
	D3DXCOLOR GetProperty
	(
		FbxSurfaceMaterial* material
		, const char* name, const char* factorName
		, string* textureName
	);
	D3DXCOLOR GetProperty
	(
		FbxSurfaceMaterial* material
		, const char* name
		, string* textureName
	);

	UINT number;
	string name;
	string shaderFile;

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	D3DXCOLOR emissive;
	D3DXCOLOR normal;
	float shininess;

	string ambientFile;
	string diffuseFile;
	string specularFile;
	string emissiveFile;
	string normalFile;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxPartData;
class FbxModelData;
class FbxModelMeshData
{
public:
	FbxModelMeshData(FbxPartData* part, FbxMaterialData* material);
	~FbxModelMeshData();

	FbxMaterialData* GetMaterial() { return material; }

	void AddVertex(D3DXVECTOR3& position, D3DXVECTOR3& normal, D3DXVECTOR2& uv, const FbxBoneWeightsData& boneWeight);
	void AddVertex(D3DXVECTOR3& position, D3DXVECTOR3& normal, D3DXVECTOR2& uv);

	void Write(BinaryWriter* w);

private:
	void CalcTBB();

	friend class MoLoader;
	friend class MoModel;

	FbxPartData* part;
	FbxModelData* model;
	FbxMaterialData* material;

	typedef VertexTextureNormalTangentBinormalBlend VertexType;

	// 저장할 애들;
	vector<VertexType> vertices;
	vector<UINT> indices;
	vector<FbxBoneWeightsData> boneWeights;

	bool isSkinnedModel;

	//얘는 직접 그릴얘가 아니니 버퍼를 만들필요없이 vertices와 indices, boneWeights를 가지고 있으면 된다
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxPartData
{
public:
	FbxPartData(string name);
	FbxPartData();
	~FbxPartData();

	void AddVertex(FbxMaterialData*, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2, const FbxBoneWeightsData&);
	void AddVertex(FbxMaterialData*, D3DXVECTOR3, D3DXVECTOR3, D3DXVECTOR2);

	void SetGeometricOffset(const D3DXMATRIX& matGeometricOffset)
	{
		this->matGeometricOffset = matGeometricOffset;
	}

	D3DXMATRIX GetGeometricOffset()
	{
		return matGeometricOffset;
	}

	D3DXMATRIX GetAnimationTransform()
	{
		return matAnimationTransform;
	}

	void SetAbsoluteTransform(const D3DXMATRIX& matAbsoluteTransform)
	{
		this->matAnimationTransform = matAbsoluteTransform;
	}

	void Write(BinaryWriter* w);

	string GetName() { return name; }

	typedef pair<string, FbxAnimationKeyFrameDatas*> Pair;
	void AddAnimationKeyFrames(Pair data) { animationKeyFrames.push_back(data); }
	
private:
	//요넘들은 저장해야함
	string name;
	vector<FbxModelMeshData *> meshes;

	
	vector<Pair> animationKeyFrames;

	D3DXMATRIX matGeometricOffset;
	D3DXMATRIX matAnimationTransform;

};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxModelData
{
private:
	friend class FbxLoader;

	FbxModelData(string file);
	FbxModelData();
	~FbxModelData();

	void PushMaterial(FbxMaterialData* material);

	D3DXMATRIX GetAbsoluteTransformFromCurrentTake(FbxNode* node, FbxTime time);

	void Write(string file);
	void Write(wstring file);

	string file;

	D3DXMATRIX geometricOffset;

	FbxSkeletonData* skeleton;
	FbxAnimationControllerData* animationController;

	vector<FbxMaterialData *> materials;
	vector<FbxPartData* > parts;
};

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

class FbxModelUtility
{
public:
	static D3DXVECTOR2 ToVector2(const FbxVector2& vec)
	{
		return D3DXVECTOR2((float)vec.mData[0], (float)vec.mData[1]);
	}

	static D3DXVECTOR3 ToVector3(const FbxVector4& vec)
	{
		return D3DXVECTOR3((float)vec.mData[0], (float)vec.mData[1], (float)vec.mData[2]);
	}

	static D3DXCOLOR ToColor(const FbxVector4& vec)
	{
		return D3DXCOLOR((float)vec.mData[0], (float)vec.mData[1], (float)vec.mData[2], 1);
	}

	static D3DXVECTOR2 ToUv(const FbxVector2& vec)
	{
		D3DXVECTOR2 temp = ToVector2(vec);

		return D3DXVECTOR2(temp.x, 1.0f - temp.y);
	}

	static D3DXMATRIX ToMatrix(const FbxMatrix& matrix)
	{
		FbxVector4 r1 = matrix.GetRow(0);
		FbxVector4 r2 = matrix.GetRow(1);
		FbxVector4 r3 = matrix.GetRow(2);
		FbxVector4 r4 = matrix.GetRow(3);

		return D3DXMATRIX
		(
			(float)r1.mData[0], (float)r1.mData[1], (float)r1.mData[2], (float)r1.mData[3],
			(float)r2.mData[0], (float)r2.mData[1], (float)r2.mData[2], (float)r2.mData[3],
			(float)r3.mData[0], (float)r3.mData[1], (float)r3.mData[2], (float)r3.mData[3],
			(float)r4.mData[0], (float)r4.mData[1], (float)r4.mData[2], (float)r4.mData[3]
		);
	}
};