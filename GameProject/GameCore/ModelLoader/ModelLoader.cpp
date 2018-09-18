#include "stdafx.h"
#include "./ModelLoader.h"

#include "../../Model/Model.h"
#include "../../Model/ModelMaterial.h"
#include "../../Model/ModelMesh.h"
#include "../../Model/Animation/ModelAnimation.h"
#include "../../Model/Animation/ModelAnimationController.h"
#include "../../Model/Animation/ModelAnimationKeyFrames.h"
#include "../../Model/Skeleton/ModelSkeleton.h"
#include "../../Model/Skeleton/ModelSkeletonBone.h"

#include "../Collider/Collider.h"
#include "../Collider/Capsule/CapsuleCollider.h"

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

void ModelLoader::ReadMaterial(BinaryReader * r, ModelMaterial * modelMaterial)
{
	modelMaterial->number = r->UInt();

	D3DXCOLOR color;

	color = r->Color4f();
	modelMaterial->SetAmbient(color);

	color = r->Color4f();
	modelMaterial->SetDiffuse(color);

	color = r->Color4f();
	modelMaterial->SetSpecular(color);

	color = r->Color4f();
	modelMaterial->SetEmissive(color);

	modelMaterial->shininess = r->Float();


	string file = "";

	file = r->String();
	modelMaterial->SetAmbientTexture(String::StringToWString(file));

	file = r->String();
	modelMaterial->SetDiffuseTexture(String::StringToWString(file));

	file = r->String();
	modelMaterial->SetSpecularTexture(String::StringToWString(file));

	file = r->String();
	modelMaterial->SetEmissiveTexture(String::StringToWString(file));
}

void ModelLoader::ReadModel(BinaryReader * r, Model * model)
{
	r->Matrix(); //matGeometricOffset
	r->Matrix(); //matAnimationTransform

	//메쉬
	UINT count = r->UInt();
	for (UINT i = 0; i < count; ++i)
	{
		UINT index = r->UInt();
		ModelMesh* part = new ModelMesh(index);
		ReadModelMesh(r, part);

		model->parts.push_back(part);
	}
}

void ModelLoader::ReadModelMesh(BinaryReader * r, ModelMesh * modelMesh)
{
	modelMesh->vertexCount = r->UInt();
	VertexTextureNormalBlend* vertex = new VertexTextureNormalBlend[modelMesh->vertexCount];
	r->Byte((void**)&vertex,
		sizeof(VertexTextureNormalBlend)*modelMesh->vertexCount);

	modelMesh->indexCount = r->UInt();
	UINT* index = new UINT[modelMesh->indexCount];
	r->Byte((void **)&index, sizeof(UINT) * modelMesh->indexCount);

	modelMesh->isSkinnedModel = r->Bool();

	modelMesh->CreateBuffer(vertex, index);

	SAFE_DELETE_ARRAY(vertex);
	SAFE_DELETE_ARRAY(index);
}

void ModelLoader::ReadSkeleton(BinaryReader * r, ModelSkeleton * skeleton, vector<ModelAnimation*>& animationClips)
{
	skeleton->boneCount = r->Int();
	UINT count = r->UInt();
	for (UINT i = 0; i < count; ++i)
	{
		pair<wstring, ModelSkeletonBone*> pair;
		pair.first = String::StringToWString(r->String());

		string name = r->String();
		int parentIndex = r->Int();

		pair.second = new ModelSkeletonBone(String::StringToWString(name), parentIndex);
		pair.second->matBindPoseTransform = r->Matrix();
		pair.second->matInvBindPoseTransform = r->Matrix();
		pair.second->matBoneReferenceTransform = r->Matrix();
		pair.second->matInvBoneReferenceTransform = r->Matrix();

		//애니메이션 클립에 KeyFrameData 입력.
		UINT animationCount = r->UInt(); //keyFrameCount == 애니메이션 개수 (하지만 안씀;)
		for (UINT j = 0; j < animationClips.size(); ++j)
		{
			string name = r->String();
			ModelAnimationKeyFrames* keyFrames = new ModelAnimationKeyFrames(String::StringToWString(name));
			ReadKeyFrames(r, keyFrames);

			animationClips[j]->animationKeyFrames.push_back(keyFrames);
		}

		skeleton->skeletonBones.push_back(pair);
	}
}

void ModelLoader::ReadKeyFrames(BinaryReader * r, ModelAnimationKeyFrames * dest)
{
	UINT count = r->UInt();
	for (UINT i = 0; i < count; ++i)
	{
		D3DXMATRIX mat = r->Matrix();
		D3DXVECTOR4 q = r->Vector4();
		D3DXVECTOR3 t = r->Vector3();

		ModelAnimationQuaternionKeyFrame quaternionKeyFrame(q, t);
		dest->keyFrames.push_back(make_pair(mat, quaternionKeyFrame));
	}
}

void ModelLoader::ReadAnimation(BinaryReader * r, ModelAnimationController * animationController)
{
	UINT count = r->UInt();
	for (UINT i = 0; i < count; ++i)
	{
		string name = r->String();
		string animationName = r->String();
		int keyFrameCount = r->Int();
		float defaultFrameRate = r->Float();

		ModelAnimation* anim = new ModelAnimation(String::StringToWString(animationName),
			keyFrameCount, defaultFrameRate);
		anim->frameRate = r->Float();

		animationController->AddAnimation(anim);
	}
	if (count > 0)
	{
		animationController->currentAnimation = animationController->animations[count - 1].second;
		animationController->Play();
	}
}

void ModelLoader::ReadCollider(BinaryReader * r, Collider ** collider)
{
	int type = r->Int();
	switch (type)
	{
	case Collider::CAPSULE:
	{
		ReadCapsuleCollider(r, collider);
	}
	break;
	default:
	{
		(*collider) = new CapsuleCollider(0.5f, 1.0f);
	}
	break;
	}
	(*collider)->SetName(String::StringToWString(r->String()));
	(*collider)->SetPosition(r->Vector3());

	D3DXVECTOR4 q = r->Vector4();
	(*collider)->SetRotation({ q.x, q.y, q.z, q.w });
}

void ModelLoader::ReadCapsuleCollider(BinaryReader * r, Collider ** collider)
{
	float height = r->Float();
	float radius = r->Float();

	*collider = new CapsuleCollider(radius, height);
}
