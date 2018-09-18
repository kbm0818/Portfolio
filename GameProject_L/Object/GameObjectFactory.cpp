#include "stdafx.h"
#include "GameObjectFactory.h"
#include "GameObject.h"

#include "Components/ComponentFactory.h"
#include "Mesh/MeshFactory.h"
#include "Shader/ShaderFactory.h"
#include "SRV/SRVFactory.h"
#include "Mesh\TerrainMeshFactory.h"
#include "Mesh\MeshNodeData.h"

#include "Components\MaterialComponent.h"
#include "Shader\Shader.h"

#include "Fbx\FbxLoader.h"

#include "Model\Model.h"

GameObjectFactory::GameObjectFactory()
{
	componentFactory = new ComponentFactory;
	meshFactory = new MeshFactory;
	terrainMeshFactory = new TerrainMeshFactory;
}

GameObjectFactory::~GameObjectFactory()
{
	SAFE_DELETE(componentFactory);
	SAFE_DELETE(meshFactory);

	SAFE_DELETE(terrainMeshFactory);
}

GameObject * GameObjectFactory::Create(string objectName, GlobalValues* values)
{
	GameObject* ob = new GameObject;
	
	if (strcmp(objectName.c_str(), "Terrain") == 0)
	{
		ob->SetMesh(terrainMeshFactory->Create(values));

		ob->SetComponent(componentFactory->Create(TransformComponentType::TransformComponent, ob));
		
		ob->SetComponent(componentFactory->Create(InputControllerType::NONE, ob));

		pair<ComponentFamilyID,IComponent*> materialC = componentFactory->Create(MaterialComponentType::MaterialComponent, ob);
		MaterialComponent* temp = dynamic_cast<MaterialComponent*>(materialC.second);
		if (temp != nullptr)
		{
			temp->SetShader(values->ShaderFactory->Create(Shaders + L"GameTerrain_Bump.hlsl", ShaderType::VP), ShaderType::VP);
			
			temp->AddSRV(values->SRVFactory->Create(Textures + L"grass.dds"));
			temp->AddSRV(values->SRVFactory->Create(Textures + L"detail.dds"));
			temp->AddSRV(values->SRVFactory->Create(Textures + L"slope.dds"));
			temp->AddSRV(values->SRVFactory->Create(Textures + L"rock.dds"));
			temp->AddSRV(values->SRVFactory->Create(Textures + L"bump.dds"));

		}
		ob->SetComponent(materialC);
	} else if (strcmp(objectName.c_str(), "Box") == 0)
	{
		ob->SetMesh(meshFactory->Create<VertexTextureNormalTangentBinormal>(Contents + L"UserBinaryMeshes/Cube.bin", MeshType::BinaryCustumFile));

		ob->SetComponent(componentFactory->Create(TransformComponentType::Transform_PhysiscComponent, ob));

		ob->SetComponent(componentFactory->Create(InputControllerType::UserControllerComponent, ob));

		pair<ComponentFamilyID, IComponent*> materialC = componentFactory->Create(MaterialComponentType::MaterialComponent, ob);
		MaterialComponent* temp = dynamic_cast<MaterialComponent*>(materialC.second);
		if (temp != nullptr)
		{
			temp->SetShader(values->ShaderFactory->Create(Shaders + L"POMTestRender.hlsl", ShaderType::VP), ShaderType::VP);
			temp->AddSRV(values->SRVFactory->Create(Textures + L"toy_box_disp.png"));
			temp->AddSRV(values->SRVFactory->Create(Textures + L"toy_box_normal.png"));
			temp->AddSRV(values->SRVFactory->Create(Textures + L"Box.png"));
		}
		ob->SetComponent(materialC);
	}
	else if (strcmp(objectName.c_str(), "SkyDome") == 0)
	{
		IMeshData* mesh = meshFactory->Create<Vertex>(Contents + L"Meshes/Sphere.data", MeshType::BinaryMeshFile);
		mesh->SetDrawOption(MeshRenderOption::D_DepthOff | MeshRenderOption::R_CullOff);
		ob->SetMesh(mesh);

		ob->SetComponent(componentFactory->Create(TransformComponentType::Transfrom_FollowCamera, ob));

		pair<ComponentFamilyID, IComponent*> materialC = componentFactory->Create(MaterialComponentType::MaterialComponent, ob);
		MaterialComponent* temp = dynamic_cast<MaterialComponent*>(materialC.second);
		if (temp != nullptr)
		{
			temp->SetShader(values->ShaderFactory->Create(Shaders + L"Sky.hlsl", ShaderType::VP), ShaderType::VP);
			temp->AddColor(D3DXCOLOR(0.0f, 0.05f, 0.6f, 1.0f));
			temp->AddColor(D3DXCOLOR(0.0f, 0.5f, 0.8f, 1.0f));
		}
		ob->SetComponent(materialC);
	}
	else if (strcmp(objectName.c_str(), "SkyPlane") == 0)
	{
		IMeshData* mesh = meshFactory->Create<VertexTexture>(Contents + L"UserBinaryTerrains/SkyPlane.map", MeshType::BinaryCustumFile);
		mesh->SetDrawOption(MeshRenderOption::D_DepthOff | MeshRenderOption::B_BlendOn);
		ob->SetMesh(mesh);

		ob->SetComponent(componentFactory->Create(TransformComponentType::Transfrom_FollowCamera, ob));

		pair<ComponentFamilyID, IComponent*> materialC = componentFactory->Create(MaterialComponentType::MaterialComponent, ob);
		MaterialComponent* temp = dynamic_cast<MaterialComponent*>(materialC.second);
		if (temp != nullptr)
		{
			temp->SetShader(values->ShaderFactory->Create(Shaders + L"SkyPlaneWithPerlin.hlsl", ShaderType::VP), ShaderType::VP);
			temp->AddSRV(values->SRVFactory->Create(Textures + L"cloud001.dds"));
			temp->AddSRV(values->SRVFactory->Create(Textures + L"perlin.bmp"));
			temp->SetSamplerType(SamplerType::MIRROR);
		}
		ob->SetComponent(materialC);
		ob->SetComponent(componentFactory->Create(CustumBufferType::SkyPlane, ob));
	}

	return ob;
}

GameObject * GameObjectFactory::CreateModel(wstring modelName, GlobalValues * values)
{
	GameObject* ob = new GameObject;

	IMeshData* mesh = FbxLoader::Load(modelName, values);
	Model* model = dynamic_cast<Model*>(mesh);
	bool check = FbxLoader::LoadAnim(L"SambaDancing", model);
	assert(check);

	model->PlayAnimation(0);
	ob->SetMesh(mesh);

	ob->SetComponent(componentFactory->Create(TransformComponentType::Transform_PhysiscComponent, ob));
	ob->SetComponent(componentFactory->Create(InputControllerType::UserControllerComponent, ob));

	return ob;
}
