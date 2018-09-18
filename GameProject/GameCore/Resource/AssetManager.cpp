#include "stdafx.h"
#include "AssetManager.h"
#include "../ModelLoader/ModelLoader.h"
#include "../../Model/Model.h"
#include "../../Model/ModelMaterial.h"
#include "../../Model/ModelMesh.h"
#include "../../Model/Animation/ModelAnimation.h"
#include "../../Model/Animation/ModelAnimationKeyFrames.h"
#include "../../Model/Animation/ModelAnimationController.h"
#include "../../Model/Skeleton/ModelSkeleton.h"
#include "../../Model/Skeleton/ModelSkeletonBone.h"

#include "./GameCore/Resource/Mesh.h"
#include "./GameCore/Resource/Material.h"
#include "./GameCore/GameObject/Components/Renderer/MeshFilter.h"
#include "./GameCore/GameObject/Components/Renderer/Renderer.h"
#include "./GameCore/GameObject/Components/Renderer/SkinnedMeshRenderer.h"
#include "./GameCore/GameObject/Components/Animator/Animator.h"

#include "./GameCore/GameObject/Components/Transform.h"
#include "./GameCore/Collider/ColliderManager.h"

#include "./GameCore/Collider/Collider.h"
#include "./GameCore/Collider/Capsule/CapsuleCollider.h"

AssetManager* AssetManager::instance = nullptr;

AssetManager * AssetManager::Get()
{
	return instance;
}

void AssetManager::Create()
{
	instance = new AssetManager();

	//TODO: 쉐이더 로드는 여기서

	//기본 쉐이더 로드
	AssetManager::Get()->AddShader<Shader,VertexColor>("VertexColor", Shaders + L"VertexColor.hlsl");
	AssetManager::Get()->AddShader<Shader,VertexTexture>("BasicTexture", Shaders + L"BaseShader.hlsl");
	//AssetManager::Get()->AddShader<Shader,VertexTextureNormalBlend>("BasicModel", Shaders + L"Model.hlsl");

	//기본 메터리얼 로드
	AssetManager::Get()->AddMaterial("DefaultMaterial");
}

void AssetManager::Delete()
{
	SAFE_DELETE(instance);
}

void AssetManager::AddModel(string name, string filePath)
{
	BinaryReader* r = new BinaryReader();
	wstring temp = String::StringToWString(filePath);

	UINT count = 0;
	r->Open(temp);
	{
		string filePath = r->String(); //ModelScene FBX 파일 경로
		ModelData* modelData = new ModelData;

		// Material 추가
		count = r->UInt();
		for (UINT i = 0; i < count; ++i)
		{
			ModelMaterial* mat = new ModelMaterial();
			ModelLoader::ReadMaterial(r, mat);
			modelData->embeddedMaterials.push_back(mat->MoveMaterial());
			SAFE_DELETE(mat);
		}	

		// Mesh 데이터 추가
		count = r->UInt();
		for (UINT i = 0; i < count; ++i)
		{
			string name = r->String();
			Model* model = new Model(String::StringToWString(name));
			ModelLoader::ReadModel(r, model);

			vector<ModelMesh*>& partList = model->GetPartsList();
			for (UINT j = 0; j<partList.size(); ++j)
			{
				MeshData meshData;
				partList[j]->MoveMeshData(&meshData);
				Mesh* mesh = new Mesh(meshData);

				ModelData::MeshInfo info;
				info.isSkinning = partList[j]->IsSkinning();
				info.materialIndex = partList[j]->GetMaterialIndex();
				info.rootBoneIndex = partList[j]->GetJointIndex();

				modelData->meshes.push_back(make_pair(info,mesh));
				SAFE_DELETE(partList[j]);
			}
			SAFE_DELETE(model);
		}

		// Skeleton 및 애니메이션 클립
		//Animation Clip
		bool bExistAnimation = r->Bool();
		if (bExistAnimation)
		{
			UINT count = r->UInt();
			for (UINT i = 0; i < count; ++i)
			{
				string name = r->String();
				string animationName = r->String();
				int keyFrameCount = r->Int();
				float defaultFrameRate = r->Float();
				float frameRate = r->Float();

				ModelAnimation* anim = new ModelAnimation(String::StringToWString(animationName),
					keyFrameCount, defaultFrameRate, frameRate);

				modelData->embeddedAnimations.push_back(anim);
			}
		}

		bool bExistSkeleton = r->Bool();
		modelData->skeleton = nullptr;
		if (bExistSkeleton)
		{
			ModelSkeleton* skeleton = new ModelSkeleton();
			ModelLoader::ReadSkeleton(r, skeleton, modelData->embeddedAnimations);
			modelData->skeleton = skeleton;
		}

		//Colliders
		bool bExistCollider = r->Bool();
		int colliderCount = r->Int();
		if (bExistCollider)
		{
			for (int i = 0; i < colliderCount; ++i)
			{
				int boneIndex = r->Int();
				Collider* col;
				ModelLoader::ReadCollider(r, &col);
				modelData->embeddedColliders.push_back(make_pair(boneIndex, col));
			}
		}

		modelContainer.insert(make_pair(name, modelData));
	}
	r->Close();

	SAFE_DELETE(r);

}


void AssetManager::AddMesh(string name, Mesh * mesh)
{
	meshContainer[name] = mesh;
}

void AssetManager::AddMaterial(string name, wstring diffuse, wstring specular, wstring ambient, wstring emissive, 
	D3DXCOLOR diffuseColor, D3DXCOLOR specularColor, D3DXCOLOR ambientColor, D3DXCOLOR emissiveColor, int shininess)
{
	Material* mat = new Material();
	mat->SetAmbient(ambientColor);
	mat->SetDiffuse(diffuseColor);
	mat->SetEmissive(emissiveColor);
	mat->SetSpecular(specularColor);
	mat->SetShininess((float)shininess);

	mat->SetDiffuseTexture(diffuse);
	mat->SetSpecularTexture(specular);
	mat->SetAmbientTexture(ambient);
	mat->SetEmissiveTexture(emissive);

	materialContainer[name] = mat;
}

Shader * AssetManager::GetShader(string name)
{
	return shaderContainer[name];
}

Mesh * AssetManager::GetMesh(string name)
{
	return meshContainer[name];
}

Material * AssetManager::GetMaterial(string name)
{
	if (name == "") name = "DefaultMaterial";
	return materialContainer[name];
}

void AssetManager::SetupToLoadedModel(string name, GameObject** object)
{
	ModelData* data = modelContainer[name];
	if (!data) return;

	*object = new GameObject;
	GameObject* obj = *object;
	obj->name = name;

	//애니메이터
	Animator* animator = obj->AddComponent<Animator>();
	ModelAnimationController* controller = animator->GetAnimationController();
	for (size_t i = 0; i < data->embeddedAnimations.size(); ++i)
	{
		controller->AddAnimation(data->embeddedAnimations[i]);
	}
	animator->CopySkeleton(data->skeleton);
	ModelSkeleton* skeleton = animator->GetSkeleton();

	//메쉬
	for (size_t i = 0; i < data->meshes.size(); ++i)
	{
		GameObject* meshObj = new GameObject;
		MeshFilter* meshFilter = meshObj->AddComponent<MeshFilter>();
		meshFilter->SetMesh(data->meshes[i].second);

		ModelData::MeshInfo info = data->meshes[i].first;
		Material* mat = data->embeddedMaterials[info.materialIndex];
		SkinnedMeshRenderer* meshRenderer = meshObj->AddComponent<SkinnedMeshRenderer>(); 
		meshRenderer->Init(mat, animator->GetBoneBuffer(), info.rootBoneIndex, info.isSkinning);

		obj->GetTransform()->AddChild(meshObj->GetTransform());
	}

	//컬라이더
	if (data->embeddedColliders.size() > 0)
	{
		ColliderManager* colliderManager = obj->AddComponent<ColliderManager>();
		Collider* collider = nullptr;
		for (size_t i = 0; i < data->embeddedColliders.size(); ++i)
		{
			collider = data->embeddedColliders[i].second;
			if (collider->GetType() == Collider::CAPSULE)
			{
				CapsuleCollider* capsuleCollider = new CapsuleCollider(*(CapsuleCollider*)collider);
				colliderManager->PushCollider(capsuleCollider, data->embeddedColliders[i].first);
			}
		}
	}

}

void AssetManager::SetupMesh(string mesh, string material, GameObject ** object)
{
	*object = new GameObject;
	GameObject* obj = *object;

	MeshFilter* meshFilter = obj->AddComponent<MeshFilter>();
	meshFilter->SetMesh(meshContainer[mesh]);

	Renderer* renderer = obj->AddComponent<Renderer>();
	renderer->Init("BasicTexture", material);
}

AssetManager::AssetManager()
{

}

AssetManager::~AssetManager()
{
	for (auto iter = modelContainer.begin(); iter != modelContainer.end(); ++iter)
		SAFE_DELETE((*iter).second);
	for (auto iter = shaderContainer.begin(); iter != shaderContainer.end(); ++iter)
		SAFE_DELETE((*iter).second);
	for (auto iter = materialContainer.begin(); iter != materialContainer.end(); ++iter)
		SAFE_DELETE((*iter).second);
	for (auto iter = animationContainer.begin(); iter != animationContainer.end(); ++iter)
		SAFE_DELETE((*iter).second);
}
