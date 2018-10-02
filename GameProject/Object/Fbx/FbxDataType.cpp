#include "stdafx.h"
#include "FbxDataType.h"

/////////////////////////////FbxAnimationQuaternionKeyFrameData/////////////////////////////////

void FbxAnimationQuaternionKeyFrameData::Write(BinaryWriter * w)
{
	w->QUATERNION(q);
	w->Vector3(translation);
}

/////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////FbxAnimationKeyFrameDatas////////////////////////////////////

FbxAnimationKeyFrameDatas::FbxAnimationKeyFrameDatas(wstring animationName)
	: animationName(animationName)
{
}

FbxAnimationKeyFrameDatas::~FbxAnimationKeyFrameDatas()
{
}

void FbxAnimationKeyFrameDatas::AddKeyFrame(const D3DXMATRIX & transform)
{
	keyFrames.push_back(make_pair(transform, FbxAnimationQuaternionKeyFrameData(transform)));
}

wstring FbxAnimationKeyFrameDatas::GetAnimationName()
{
	return animationName;
}

const D3DXMATRIX & FbxAnimationKeyFrameDatas::GetKeyFrameTransform(int keyFrame) const
{
	return keyFrames[keyFrame].first;
}

const FbxAnimationQuaternionKeyFrameData & FbxAnimationKeyFrameDatas::GetQuaternionKeyFrame(int keyFrame) const
{
	return keyFrames[keyFrame].second;
}

void FbxAnimationKeyFrameDatas::Write(BinaryWriter * w)
{
	w->WString(animationName);

	w->UInt(keyFrames.size());
	for (size_t i = 0; i < keyFrames.size(); i++)
	{
		w->Matrix(keyFrames[i].first);
		keyFrames[i].second.Write(w);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxAnimationData::FbxAnimationData(wstring name, int keyFrame, float frameRate)
	: name(name), keyFrame(keyFrame), frameRate(frameRate), defaultFrameRate(frameRate)
{
}

FbxAnimationData::~FbxAnimationData()
{
}

void FbxAnimationData::Write(BinaryWriter * r)
{
	r->WString(name);
	r->Int(keyFrame);
	r->Float(frameRate);
	r->Float(defaultFrameRate);
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxAnimationControllerData::FbxAnimationControllerData()
{
	currentAnimation = NULL;
	currentKeyFrame = 0;
	nextKeyFrame = 0;
	keyFrameFactor = 0.0f;
	frameTimer = 0.0f;

	useQuaternionKeyFrames = true;
	useOld = false;
	animationMode = FbxAnimationMode::Stop;

	curAnimName = L"";
}

FbxAnimationControllerData::~FbxAnimationControllerData()
{
	for each(Pair temp in animations)
		SAFE_DELETE(temp.second);

	currentAnimation = NULL;
}

void FbxAnimationControllerData::AddAnimation(FbxAnimationData * animation)
{
	bool isExist = false;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animation->GetName())
		{
			isExist = true;

			break;
		}
	}
	assert(isExist == false);

	animations.push_back(Pair(animation->GetName(), animation));
}

FbxAnimationData * FbxAnimationControllerData::GetCurrentAnimation()
{
	if (currentAnimation != NULL)
		return currentAnimation;

	return NULL;
}

void FbxAnimationControllerData::SetCurrentAnimation(wstring animationName)
{
	//이미 실행하고 있는 애니와 같다면 실행하지 않겠다
	if (wcscmp(curAnimName.c_str(), animationName.c_str()) == 0)
		return;

	curAnimName = animationName;
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}

	if (count != -1)
		currentAnimation = animations[count].second;
	else
		currentAnimation = NULL;

	Stop();
	Play();
}

void FbxAnimationControllerData::SetCurrentAnimation(int index)
{
	currentAnimation = animations[index].second;

	Stop();
}

UINT FbxAnimationControllerData::GetAnimationCount()
{
	return animations.size();
}

FbxAnimationData * FbxAnimationControllerData::GetAnimation(int index)
{
	return animations[index].second;
}

FbxAnimationData * FbxAnimationControllerData::GetAnimation(wstring animationName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}

	if (count != -1)
		return animations[count].second;
	else
		return NULL;
}

void FbxAnimationControllerData::ChangeOldThing()
{
	if (defaultAnimation == NULL)
		return;

	currentAnimation = defaultAnimation;
	useOld = false;

	Stop();
	Play();
}

void FbxAnimationControllerData::SetDefaultAnim(wstring animationName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}

	if (count == -1)
		return;

	defaultAnimation = animations[count].second;
}

void FbxAnimationControllerData::UsePopAnimation(wstring animationName)
{
	UINT count = -1;
	for (size_t i = 0; i < animations.size(); i++)
	{
		if (animations[i].first == animationName)
		{
			count = i;
			break;
		}
	}

	if (count == -1)
		return;

	currentAnimation = animations[count].second; // 쓰려는걸로 바꾸고
	useOld = true;

	Stop();
	Play();
}

void FbxAnimationControllerData::Play()
{
	animationMode = FbxAnimationMode::Play;
}

void FbxAnimationControllerData::Pause()
{
	animationMode = FbxAnimationMode::Pause;
}

void FbxAnimationControllerData::Stop()
{
	animationMode = FbxAnimationMode::Stop;
	frameTimer = 0.0f;

	SetCurrentKeyFrame(0);
}

void FbxAnimationControllerData::SetCurrentKeyFrame(int keyFrame)
{
	currentKeyFrame = (keyFrame) % currentAnimation->GetKeyFrames();
	nextKeyFrame = (currentKeyFrame + 1) % currentAnimation->GetKeyFrames();

	keyFrameFactor = 0.0f;
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

const UINT FbxBoneWeightsData::MaxBonesPerVertex = 4;
const UINT FbxBoneWeightsData::MaxBonesPerMax = 100;

FbxBoneWeightsData::FbxBoneWeightsData()
	: sumWeight(0.0f)
{
}

FbxBoneWeightsData::~FbxBoneWeightsData()
{
}

void FbxBoneWeightsData::AddBoneWeight(int boneIndex, float boneWeight)
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

void FbxBoneWeightsData::AddBoneWeight(pair<int, float> boneWeightPair)
{
	AddBoneWeight(boneWeightPair.first, boneWeightPair.second);
}

void FbxBoneWeightsData::AddBoneWeights(const FbxBoneWeightsData & boneWeights)
{
	for (int i = 0; i < boneWeights.GetBoneWeightCount(); ++i)
	{
		AddBoneWeight(boneWeights.GetBoneWeight(i));
	}
}

void FbxBoneWeightsData::Validate()
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

void FbxBoneWeightsData::Normalize()
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

FbxBlendWeightsData FbxBoneWeightsData::GetBlendWeights()
{
	FbxBlendWeightsData blendWeights;

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

void FbxBoneWeightsData::Write(BinaryWriter * w)
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

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxSkeletonBoneData::FbxSkeletonBoneData(wstring name, int parentBoneIndex)
	: name(name), parentBoneIndex(parentBoneIndex)
{
}

FbxSkeletonBoneData::~FbxSkeletonBoneData()
{
	for each(Pair temp in animationKeyFrames)
		SAFE_DELETE(temp.second);
}

void FbxSkeletonBoneData::AddAnimationKeyFrames(FbxAnimationKeyFrameDatas * keyFrames)
{
	animationKeyFrames.push_back(Pair(keyFrames->GetAnimationName(), keyFrames));
}

void FbxSkeletonBoneData::SetBindPoseTransform(const D3DXMATRIX & matBindPoseTransform)
{
	this->matBindPoseTransform = matBindPoseTransform;
	D3DXMatrixInverse(&this->matInvBindPoseTransform, NULL, &this->matBindPoseTransform);
}

void FbxSkeletonBoneData::SetBoneReferenceTransform(const D3DXMATRIX & matBoneReferenceTransform)
{
	this->matBoneReferenceTransform = matBoneReferenceTransform;
	D3DXMatrixInverse(&this->matInvBoneReferenceTransform, NULL, &this->matBoneReferenceTransform);
}

FbxAnimationKeyFrameDatas * FbxSkeletonBoneData::GetAnimationKeyFrames(wstring animationName)
{
	for each(Pair temp in animationKeyFrames)
	{
		if (temp.first == animationName)
			return temp.second;
	}

	return NULL;
}

void FbxSkeletonBoneData::Write(BinaryWriter * w)
{
	w->WString(name);

	w->Matrix(matBindPoseTransform);
	w->Matrix(matInvBindPoseTransform);
	w->Matrix(matBoneReferenceTransform);
	w->Matrix(matInvBoneReferenceTransform);

	w->UInt(parentBoneIndex);

	w->UInt(animationKeyFrames.size());
	for (size_t i = 0; i < animationKeyFrames.size(); i++)
	{
		w->WString(animationKeyFrames[i].first);
		animationKeyFrames[i].second->Write(w);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxSkeletonData::FbxSkeletonData()
{
	skinTransforms = NULL;
	boneAnimationTransforms = NULL;

	boneCount = 0;
}

FbxSkeletonData::~FbxSkeletonData()
{
	for each(Pair temp in skeletonBones)
		SAFE_DELETE(temp.second);

	SAFE_DELETE_ARRAY(boneAnimationTransforms);
	SAFE_DELETE_ARRAY(skinTransforms);
}

void FbxSkeletonData::AddSkeletonBone(FbxSkeletonBoneData * skeletonBone)
{
	skeletonBones.push_back(Pair(skeletonBone->GetName(), skeletonBone));

	boneCount++;
}

FbxSkeletonBoneData * FbxSkeletonData::FindBone(wstring boneName)
{
	for(Pair temp : skeletonBones)
	{
		if (boneName == temp.first)
			return temp.second;
	}

	return NULL;
}

int FbxSkeletonData::FindBoneIndex(wstring boneName)
{
	int index = 0;
	for each(Pair temp in skeletonBones)
	{
		if (boneName == temp.first)
			return index;

		index++;
	}

	return -1;
}

FbxSkeletonBoneData * FbxSkeletonData::GetSkeletonBone(int index)
{
	return skeletonBones[index].second;
}

D3DXMATRIX FbxSkeletonData::GetBoneAnimationTransforms(wstring boneName)
{
	UINT index = -1;
	for (size_t i = 0; i < skeletonBones.size(); i++)
	{
		if (boneName == skeletonBones[i].first)
		{
			index = i;
			break;
		}
	}
	if (index != -1)
		return boneAnimationTransforms[index];
	else
		return D3DXMATRIX();
}

void FbxSkeletonData::Write(BinaryWriter * w)
{
	w->UInt(boneCount);

	for (UINT i = 0; i < boneCount; i++)
	{
		w->WString(skeletonBones[i].first);
		skeletonBones[i].second->Write(w);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxMaterialData::FbxMaterialData(int number, FbxSurfaceMaterial * material)
	: number(number)
	, ambient(0, 0, 0, 1), diffuse(0, 0, 0, 1), specular(0, 0, 0, 1), emissive(0, 0, 0, 1)
	, shininess(0.0f)
	, ambientFile(""), diffuseFile(""), specularFile(""), emissiveFile("")
{
	name = material->GetName();

	ambient = GetProperty(material, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, &ambientFile);
	diffuse = GetProperty(material, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, &diffuseFile);
	specular = GetProperty(material, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, &specularFile);
	emissive = GetProperty(material, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, &emissiveFile);
	normal = GetProperty(material, FbxSurfaceMaterial::sNormalMap, &normalFile);

	FbxProperty shininessProp = material->FindProperty(FbxSurfaceMaterial::sShininess);
	if (shininessProp.IsValid() == true)
		shininess = (float)shininessProp.Get<FbxDouble>();

	shaderFile = "0"; //0 문자로 쉐이더가 없을경우 기본 쉐이더로 처리
}

FbxMaterialData::~FbxMaterialData()
{
}

void FbxMaterialData::Write(BinaryWriter * w)
{
	w->UInt(number);
	w->WString(String::StringToWString(name));

	//w->WString(String::StringToWString(shaderFile));

	w->Color4f(ambient);
	w->Color4f(diffuse);
	w->Color4f(specular);
	w->Color4f(emissive);
	w->Color4f(normal);
	w->Float(shininess);

	w->WString(String::StringToWString(ambientFile));
	w->WString(String::StringToWString(diffuseFile));
	w->WString(String::StringToWString(specularFile));
	w->WString(String::StringToWString(emissiveFile));
	w->WString(String::StringToWString(normalFile));
}

D3DXCOLOR FbxMaterialData::GetProperty(FbxSurfaceMaterial * material, const char * name, const char * factorName, string * textureName)
{
	D3DXCOLOR color(0, 0, 0, 1);

	FbxProperty prop = material->FindProperty(name);
	FbxProperty factorProp = material->FindProperty(factorName);
	if (prop.IsValid() == true && factorProp.IsValid() == true)
	{
		FbxDouble3 temp = prop.Get<FbxDouble3>();
		double factor = factorProp.Get<FbxDouble>();

		color.r = (float)(temp[0] * factor);
		color.g = (float)(temp[1] * factor);
		color.b = (float)(temp[2] * factor);
	}

	if (prop.IsValid() == true)
	{
		UINT count = prop.GetSrcObjectCount<FbxFileTexture>();
		if (count > 0)
		{
			FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>();
			if (texture != NULL)
				*textureName = (const char *)texture->GetFileName();
		}
		else
		{
			*textureName = "";
		}
	}

	return color;
}

D3DXCOLOR FbxMaterialData::GetProperty(FbxSurfaceMaterial * material, const char * name, string * textureName)
{
	D3DXCOLOR color(0, 0, 0, 1);

	FbxProperty prop = material->FindProperty(name);

	if (prop.IsValid() == true)
	{
		UINT count = prop.GetSrcObjectCount<FbxFileTexture>();
		if (count > 0)
		{
			FbxFileTexture* texture = prop.GetSrcObject<FbxFileTexture>();
			if (texture != NULL)
				*textureName = (const char *)texture->GetFileName();
		}
		else
		{
			*textureName = "";
		}
	}

	return color;
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxModelMeshData::FbxModelMeshData(FbxPartData * part, FbxMaterialData * material)
	: part(part), material(material)
{
}

FbxModelMeshData::~FbxModelMeshData()
{
}

void FbxModelMeshData::AddVertex(D3DXVECTOR3 & position, D3DXVECTOR3 & normal, D3DXVECTOR2 & uv, const FbxBoneWeightsData & boneWeight)
{
	VertexType vertex;
	vertex.position = position;
	vertex.uv = uv;
	vertex.normal = normal;
	vertices.push_back(vertex);

	indices.push_back((UINT)indices.size());

	boneWeights.push_back(boneWeight);
	if (boneWeight.GetBoneWeightCount() > 0)
		isSkinnedModel = true;
}

void FbxModelMeshData::AddVertex(D3DXVECTOR3 & position, D3DXVECTOR3 & normal, D3DXVECTOR2 & uv)
{
	VertexType vertex;
	vertex.position = position;
	vertex.uv = uv;
	vertex.normal = normal;
	vertices.push_back(vertex);

	indices.push_back((UINT)indices.size());
}

void FbxModelMeshData::Write(BinaryWriter * w)
{
	CalcTBB();

	UINT materialNumber = material->GetNumber();
	w->UInt(materialNumber);

	w->UInt(vertices.size());
	w->Byte(&vertices[0], sizeof(VertexType) * vertices.size());

	w->UInt(indices.size());
	w->Byte(&indices[0], sizeof(UINT) * indices.size());

	w->Bool(isSkinnedModel);
}

void FbxModelMeshData::CalcTBB()
{
	for (UINT i = 0; i < boneWeights.size(); i++)
	{
		FbxBlendWeightsData weight = boneWeights[i].GetBlendWeights();
		vertices[i].blendIndices = weight.BlendIndices;
		vertices[i].blendWeights = weight.BlendWeights;
	}

	///// 여기서 탄젠트 계산

	for (UINT i = 0; i < ((UINT)(indices.size()) / 3); i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexTextureNormalTangentBinormalBlend& vertex0 = vertices[index0];
		VertexTextureNormalTangentBinormalBlend& vertex1 = vertices[index1];
		VertexTextureNormalTangentBinormalBlend& vertex2 = vertices[index2];

		D3DXVECTOR3 e0 = vertex1.position - vertex0.position;
		D3DXVECTOR3 e1 = vertex2.position - vertex0.position;

		float u0 = vertex1.uv.x - vertex0.uv.x;
		float u1 = vertex2.uv.x - vertex0.uv.x;

		float v0 = vertex1.uv.y - vertex0.uv.y;
		float v1 = vertex2.uv.y - vertex0.uv.y;

		float r = 1.0f / (u0 * v1 - v0 * u1);

		D3DXVECTOR3 tangent;
		tangent.x = r * (v1 * e0.x - v0 * e1.x);
		tangent.y = r * (v1 * e0.y - v0 * e1.y);
		tangent.z = r * (v1 * e0.z - v0 * e1.z);

		D3DXVECTOR3 binormal;
		binormal.x = r * (-u1 * e0.x + u0 * e1.x);
		binormal.y = r * (-u1 * e0.y + u0 * e1.y);
		binormal.z = r * (-u1 * e0.z + u0 * e1.z);

		vertex0.tangent += tangent;
		vertex1.tangent += tangent;
		vertex2.tangent += tangent;

		vertex0.binormal += binormal;
		vertex1.binormal += binormal;
		vertex2.binormal += binormal;
	}
	////////////////탄젠트 계산 끝

	for (UINT i = 0; i < vertices.size(); i++)
	{
		D3DXVec3Normalize(&vertices[i].tangent, &vertices[i].tangent);
		D3DXVec3Normalize(&vertices[i].binormal, &vertices[i].binormal);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxPartData::FbxPartData(string name)
	: name(name)
{
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

FbxPartData::FbxPartData()
{
	D3DXMatrixIdentity(&matGeometricOffset);
	D3DXMatrixIdentity(&matAnimationTransform);
}

FbxPartData::~FbxPartData()
{
	for (size_t i = 0; i < meshes.size(); i++)
		SAFE_DELETE(meshes[i]);
}

void FbxPartData::AddVertex(FbxMaterialData * material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv, const FbxBoneWeightsData& boneWeight)
{
	bool isNew = true;

	FbxModelMeshData* mesh = NULL;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		mesh = meshes[i];

		if (material == mesh->GetMaterial())
		{
			mesh->AddVertex(position, normal, uv, boneWeight);
			isNew = false;
		}
	}

	if (isNew == true)
	{
		mesh = new FbxModelMeshData(this, material);
		mesh->AddVertex(position, normal, uv, boneWeight);

		meshes.push_back(mesh);
	}
}

void FbxPartData::AddVertex(FbxMaterialData *material, D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 uv)
{
	bool isNew = true;

	FbxModelMeshData* mesh = NULL;
	for (size_t i = 0; i < meshes.size(); i++)
	{
		mesh = meshes[i];

		if (material == mesh->GetMaterial())
		{
			mesh->AddVertex(position, normal, uv);
			isNew = false;
		}
	}

	if (isNew == true)
	{
		mesh = new FbxModelMeshData(this, material);
		mesh->AddVertex(position, normal, uv);

		meshes.push_back(mesh);
	}
}

void FbxPartData::Write(BinaryWriter * w)
{
	w->String(name);

	w->UInt(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Write(w);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

FbxModelData::FbxModelData(string file)
	: file(file), animationController(nullptr)
{
	D3DXMatrixIdentity(&geometricOffset);
}
FbxModelData::FbxModelData()
	: file(""), animationController(nullptr)
{
	D3DXMatrixIdentity(&geometricOffset);
}

FbxModelData::~FbxModelData()
{
	for (FbxMaterialData* material : materials)
		SAFE_DELETE(material);
}

void FbxModelData::PushMaterial(FbxMaterialData * material)
{
	materials.push_back(material);
}

D3DXMATRIX FbxModelData::GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	return D3DXMATRIX();
}

void FbxModelData::Write(string file)
{
	Write(String::StringToWString(file));
}

void FbxModelData::Write(wstring file)
{
	BinaryWriter* w = new BinaryWriter();
	wstring temp = Contents + L"UserBinaryFbx/" + file + L"/" + file;
	// Contents/UserBinaryFbx/모델이름/모델이름.확장자

	w->Open(temp + L".mat");
	w->UInt(materials.size());
	for (FbxMaterialData* material : materials)
		material->Write(w);
	w->Close();

	w->Open(temp + L".msh");
	w->UInt(parts.size());
	for (FbxPartData* part : parts)
		part->Write(w);
	w->Close();

	w->Open(temp + L".skl");
	skeleton->Write(w);
	w->Close();

	SAFE_DELETE(w);
}

/////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////////////////

