#include "../stdafx.h"
#include "ConverterScene.h"
#include "Converter.h"
#include "ConverterMaterial.h"
#include "ConverterBuffer.h"
#include "ConverterUtility.h"
#include "ConverterAnimation.h"
#include "ConverterAnimationController.h"
#include "ConverterAnimationKeyFrames.h"
#include "ConverterSkeleton.h"
#include "ConverterSkeletonBone.h"
#include "ConverterBoneWeights.h"

#include "../Utilities/String.h"

wstring ConverterScene::TexturePath = L"";

ConverterScene::ConverterScene()
	: manager(NULL), scene(NULL), converter(NULL), importer(NULL)
	, skeleton(NULL)
{
	// TexturePath�� ����� ����
	WCHAR folder[255];
	GetCurrentDirectory(255, folder);

	TexturePath = folder;
	TexturePath += L"\\Contents\\Converters\\Textures\\";
	animationController = new ConverterAnimationController();
}

ConverterScene::~ConverterScene()
{
	for each(ConverterMaterial* temp in materials)
		SAFE_DELETE(temp);

	for each(Converter* temp in Converters)
		SAFE_DELETE(temp);

	if (scene != NULL)scene->Destroy();
	if (manager != NULL)manager->Destroy();
}

/****************************************************************************************************
@brief
Fbx SDK �� �޸� ������ ���� Manager ���� 
Fbx File�� Impoter�� ���Ͽ� import��Ų �� initialize
File�� Scene����(Material, Skeleton, Mesh, Animation) �ε�
****************************************************************************************************/
void ConverterScene::LoadFbx(wstring filePath)
{
	this->filePath = filePath;

	manager = FbxManager::Create();
	assert(manager != NULL);

	scene = FbxScene::Create(manager, "");
	assert(scene != NULL);

	string tempFile = String::WStringToString(filePath);

	//SDK�� ������ ���´�.
	int sdkMajor, sdkMinor, sdkRevision;
	FbxManager::GetFileFormatVersion(sdkMajor, sdkMinor, sdkRevision);
	
	//������ ����
	importer = FbxImporter::Create(manager, "");
	bool status = importer->Initialize(tempFile.c_str(), -1, NULL);
	assert(status == true);
	
	status = importer->Import(scene);
	assert(status == true);

	converter = new FbxGeometryConverter(manager);
	{
		ProcessScene();
	}
	SAFE_DELETE(converter);

	importer->Destroy();
}

void ConverterScene::SaveBinary(wstring filePath, wstring fileName)
{
	wstring totalFileName = filePath + fileName;

	ExportMaterials(totalFileName + L"_material.bin");
	ExportSkeleton(totalFileName + L"_skeleton.bin");
	ExportConverters(totalFileName + L"_Converters.bin");
	ExportAnimations(totalFileName + L"_animations.bin");
}

void ConverterScene::LoadBinary(wstring filePath, wstring fileName)
{
	wstring totalFileName = filePath + fileName;

	ImportMaterials(totalFileName + L"_material.bin");
	ImportSkeleton(totalFileName + L"_skeleton.bin");
	ImportConverters(totalFileName + L"_Converters.bin");
	ImportAnimations(totalFileName + L"_animations.bin");
}

ConverterAnimation * ConverterScene::GetAnimation(wstring animName)
{
	map<wstring, ConverterAnimation*>::iterator iter;
	iter = animations.find(animName);
	if (iter != animations.end())
		return iter->second;

	return NULL;
}

/****************************************************************************************************
@brief
Material, Skelton, Mesh, Animation ������ Top->Down ����� ����� ��ȸ�� �о����
****************************************************************************************************/
void ConverterScene::ProcessScene()
{
	ProcessMaterial();
	ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eSkeleton);
	ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eMesh);
	ProcessAnimations();
}

/****************************************************************************************************
@brief
Scene���� ��� Material ������ �о�, ������ Material ������ ConverterMaterial Class�� ����
****************************************************************************************************/
void ConverterScene::ProcessMaterial()
{
	for (int i = 0; i < scene->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* temp = scene->GetMaterial(i);
		
		bool isExist = false;
		for (size_t i = 0; i < fbxMaterials.size(); i++)
		{
			if (*temp == *fbxMaterials[i]) isExist = true;
		}

		if (isExist == false)
		{
			ConverterMaterial* material = new ConverterMaterial(i, temp);

			materials.push_back(material);
			fbxMaterials.push_back(temp);
		}
	}
}

/****************************************************************************************************
@brief
Node�� ������ �ε�
�� Node�� �Ӽ�(Skeelton, Mesh ���� ������ �Ӽ��� ���ʿ��ϹǷ� �ε����� ����)���� ��ȸ
�Լ� ������ ��������� ��� �ڽ� Node�� ���Ͽ� ����

@param FbxNode* node ���� ����� Root Node�� �ѹ� ����, ���� ��������� �ڽ� ��带 ����
@param FbxNodeAttribute::EType attribute ����� �Ӽ�(Skeleton, Mesh...)
****************************************************************************************************/
void ConverterScene::ProcessNode(FbxNode * node, FbxNodeAttribute::EType attribute)
{	
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != NULL)
	{
		if (nodeAttribute->GetAttributeType() == attribute)
		{
			switch (nodeAttribute->GetAttributeType())
			{
				case FbxNodeAttribute::eSkeleton:
				{
					ProcessSkeleton(node);
				}	
				break;

				case FbxNodeAttribute::eMesh:
				{
					ProcessMesh(node);
				}	
				break;

				case FbxNodeAttribute::eMarker:
				case FbxNodeAttribute::eNurbs:
				case FbxNodeAttribute::ePatch:
				case FbxNodeAttribute::eCamera:
				case FbxNodeAttribute::eLight:
				case FbxNodeAttribute::eNull:
					break;
			}//switch
		}//if
	}//if(nodeAttribute)

	for (int i = 0; i < node->GetChildCount(); i++)
		ProcessNode(node->GetChild(i), attribute);
}

/****************************************************************************************************
@brief
Bone�� ���������� ������ Skeleton ������ �ε��Ͽ� ConverterSkeleton Class�� ����
****************************************************************************************************/
void ConverterScene::ProcessSkeleton(FbxNode * node)
{
	FbxSkeleton* fbxSkeleton = node->GetSkeleton();
	if (fbxSkeleton == NULL)
		return;

	if (skeleton == NULL)
		skeleton = new ConverterSkeleton();


	int parentBoneIndex = -1;
	FbxNode* parentNode = node->GetParent();
	if (parentNode != NULL)
	{
		wstring boneName = String::StringToWString(parentNode->GetName());
		parentBoneIndex = skeleton->FindBoneIndex(boneName);
	}

	// ConverterSkeleton Class�� �� Bone�� ������ ConverterSkeletonBone Class�� ����
	wstring boneName = String::StringToWString(node->GetName());
	ConverterSkeletonBone* skeletonBone = new ConverterSkeletonBone(boneName, parentBoneIndex);
	skeleton->AddSkeletonBone(skeletonBone);
}

/****************************************************************************************************
@brief
Mesh ������ �ε��Ͽ� Converter Class�� ����
�� Mesh ����(Global Transform, Geomatric Offset)�� Converter Class��
�� Vertex�� ����(Pos, Normal, Uv, BoneWeight)�� Material ���� ������ Converter Class���� Part Class�� ����
****************************************************************************************************/
void ConverterScene::ProcessMesh(FbxNode * node)
{
	// Mesh ������ �о�� ��, �⺻ ���� ������ �ﰢ������ �б�
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	converter->Triangulate(attribute, true, true);

	FbxMesh* mesh = node->GetMesh();
	assert(mesh != NULL);

	int vertexCount = mesh->GetControlPointsCount();
	assert(vertexCount > 0);

	// Vertex�� ����ŭ ConverterBoneWeights Class�� �Ҵ�
	vector<ConverterBoneWeights> boneWeights(vertexCount, ConverterBoneWeights());
	ProcessBoneWeights(mesh, boneWeights);

	// BoneWeights�� ������ ����ִ� ConverterBuffer�� Converter -> Part���� ����
	Converter* Converter = new Converter(this, String::StringToWString(node->GetName()));

	// Control Point�� Vertex�� �������� ����, (i.e. ����ü�� ��� 4��)
	FbxVector4* controlPoints = mesh->GetControlPoints();

	// Mesh�� Polygon(�ﰢ��)����ŭ �ݺ�
	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		ConverterMaterial* material = NULL;

		// Polygon�� �� ��
		for (int vi = 0; vi < 3; vi++)
		{
			// i��° Polygon�� vi(0~2)��° Vertex�� Vertex Index�� ����
			int vertexIndex = mesh->GetPolygonVertex(i, vi);
			if (vertexIndex < 0 || vertexIndex >= vertexCount)
				continue;

			if (material == NULL)
				material = LinkMaterialWithPolygon(mesh, 0, i, 0, vertexIndex);

			FbxVector4 fbxPos = controlPoints[vertexIndex];
			FbxVector4 fbxNormal;
			mesh->GetPolygonVertexNormal(i, vi, fbxNormal);
			fbxNormal.Normalize();

			D3DXVECTOR3 vecPos = ConverterUtility::ToVector3(fbxPos);
			D3DXVECTOR3 vecNormal = ConverterUtility::ToVector3(fbxNormal);

			D3DXVECTOR2 vecUv = GetUV(mesh, 0, i, vi, vertexIndex);

			// Converter �� Vertex������ �߰�(Converter���� Material���� ������ �� Part�� �߰�)
			Converter->AddVertex(material, vecPos, vecNormal, vecUv, boneWeights[vertexIndex]);
		}//for(vi)
	}//for(i)

	 // Mesh�� Global TransForm(��Ʈ->��) �� AbsoluteTransform�� ����
	D3DXMATRIX temp = GetAbsoluteTransformFromCurrentTake(node, FbxTime(0));
	Converter->SetAnimationTransform(temp);
	// Mesh�� GeobetrixOffset�� �߰�(��κ��� ��� Identity Matrix)
	Converter->SetGeometricOffset(GetGeometricOffset(node));

	Converter->CreateData();
	Converter->CreateBuffer();

	Converters.push_back(Converter);
}

/****************************************************************************************************
@brief
Animation ������ �ε��Ͽ� ConverterAnimationController Class �� ����
****************************************************************************************************/
void ConverterScene::ProcessAnimations()
{
	FbxNode* rootNode = scene->GetRootNode();
	if (rootNode == NULL) return;

	// Fbx File ���� ������ Frame Rate(�ʴ� ������ Frame��)�� �ε�
	float frameRate = (float)FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

	// animationArray�� Animation �̸��� �ε��Ͽ� ����
	FbxArray<FbxString *> animationArray;
	FbxDocument* document = dynamic_cast<FbxDocument *>(scene);
	if (document != NULL)
		document->FillAnimStackNameArray(animationArray);

	UINT animationCount = importer->GetAnimStackCount();
	// importer�� ���� FBX�� ����� ��� Animation ������ �ε�
	for (int i = 0; i < animationCount; i++)
	{
		FbxTakeInfo* animationInfo = importer->GetTakeInfo(i);
		wstring animationName = String::StringToWString(animationInfo->mImportName.Buffer());

		// FbxTimeSpan: ���� �ð��� �����ϴ� Class
		FbxTimeSpan span = animationInfo->mLocalTimeSpan;
	
		// �� ������ ����, �� �ð��� �ε��Ͽ� �Ҵ�
		double startTime = span.GetStart().GetSecondDouble();
		double endTime = span.GetStop().GetSecondDouble();

		if (startTime < endTime)
		{
			// KeyFrames�� �� = Animation ���� �ð�(��) * �ʴ� Frmae ��
			int keyFrameCount = (int)((endTime - startTime) * (double)frameRate);

			// AnimationController�� ConverterAnimation Class�� ������ ����
			ConverterAnimation* animation = new ConverterAnimation(animationName, keyFrameCount, frameRate);
			AddAnimation(animation);

			animationController->AddAnimation(animation);
			
			// �⺻ Animation ������ �̿��Ͽ�, Skeleton, Mesh�� Animation ���� ���
			ProcessAnimation(rootNode, animationName, frameRate, (float)startTime, (float)endTime);
		}
	}
	animationArray.Clear();

	// �������� �߰��� �ִϸ��̼��� ���� �ִϸ��̼����� ���� �� �÷���
	animationController->SetCurrentAnimation(animationCount-1);
	animationController->Play();

	//TODO: defualt ���� ����
}


/****************************************************************************************************
@brief
�ش� Animation�� �� Bone�� Mesh�� ��� KeyFrame ������ ����Ͽ� animationKeyFrames�� �Ҵ��Ͽ�
�� Bone�� Mesh�� �ش��ϴ� ConverterSkeletonBone, Converter Class�� ����
****************************************************************************************************/
void ConverterScene::ProcessAnimation(FbxNode * node, wstring animationName, float frameRate, float start, float stop)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != NULL)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			if (skeleton != NULL)
			{
				wstring boneName = String::StringToWString(node->GetName());
				ConverterSkeletonBone* bone = skeleton->FindBone(boneName);

				if (bone != NULL)
				{
					ConverterAnimationKeyFrames* animationKeyFrames = new ConverterAnimationKeyFrames(animationName);

					double time = 0;
					// Animation�� ���� �ð� �ȿ���, Frame Time ���� KeyFrame�� �߰�
					// �� KeyFrame Class�� �ش� Bone�� �ش� �ð������� Local Transform Matrix(�θ� ��-> �ڽ� ��)�� �ε��Ͽ� ����

					while (time <= (double)stop)
					{
						FbxTime animationTime;
						animationTime.SetSecondDouble(time);
						
						//// Bone �ڽ��� Global Transform(�ڽ� �� -> ��Ʈ)�� ����
						//D3DXMATRIX matAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node, animationTime);
						//// �θ� Bone �� Global Transform(�θ� �� -> ��Ʈ)�� ����
						//D3DXMATRIX matParentAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node->GetParent(), animationTime);
						//
						//// �θ� Bone �� Global Transform(�θ� �� -> ��Ʈ)�� Inverse Transform(��Ʈ -> �θ� ��)�� ����
						//D3DXMATRIX matInvParentAbsoluteTransform;
						//D3DXMatrixInverse(&matInvParentAbsoluteTransform, NULL, &matParentAbsoluteTransform);
						//
						//// matTransform = (�ڽ� �� -> ��Ʈ) * (��Ʈ -> �θ� ��) == (�ڽ� �� -> �θ� ��)
						//// �θ� Bone ��ǥ�迡���� Bone�� Transform Matrix
						//D3DXMATRIX localTransform = matAbsoluteTransform * matInvParentAbsoluteTransform;
						

						// Local Transform = �θ� Bone�� Global Trasforn �� Inverse Transform * �ڽ� Bone�� Global Transform
						D3DXMATRIX localTransform = GetLocalTransformFromCurrentTake(node, animationTime);
						animationKeyFrames->AddKeyFrame(localTransform);

						time += 1.0f / frameRate;
					}

					bone->AddAnimationKeyFrames(animationKeyFrames);
				}//if(bone)
			}//if(skeleton)
		}
		else if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			Converter* Converter = NULL;
			for each(Converter* tempConverter in Converters)
			{
				wstring ConverterName = String::StringToWString(node->GetName());
				if (ConverterName == tempConverter->GetName())
				{
					Converter = tempConverter;

					break;
				}
			}

			if (Converter != NULL)
			{
				ConverterAnimationKeyFrames* animationKeyFrames = new ConverterAnimationKeyFrames(animationName);

				double time = 0;
				while (time <= (double)stop)
				{
					FbxTime animationTime;
					animationTime.SetSecondDouble(time);

					// Mesh�� ��� GlobalTransform Matrix(��Ʈ->�Ž�) ������ ����
					D3DXMATRIX matAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node, animationTime);
					animationKeyFrames->AddKeyFrame(matAbsoluteTransform);

					time += 1.0f / frameRate;
				}

				Converter->AddAnimationKeyFrames(animationKeyFrames);
			}
		}//if(nodeAttribute->GetAttributeType())
	}//if(nodeAttribute)

	// �ڽ� node(Bone)�� ��������� ��ȸ
	for (int i = 0; i < node->GetChildCount(); ++i)
		ProcessAnimation(node->GetChild(i), animationName, frameRate, start, stop);
}

/****************************************************************************************************
@brief
Mesh�� Skin ������ �ε��Ͽ�, �ش� Mesh�� �� Bone�� ���Ͽ� ��ŭ�� ������ �޴���(Weight)�� �ε�
****************************************************************************************************/
void ConverterScene::ProcessBoneWeights(FbxMesh * mesh, vector<ConverterBoneWeights>& meshBoneWeights)
{
	if (skeleton == NULL)
		return;

	// Deformer�� �ַ� Scene���� �ϳ��� ����
	for (int i = 0; i < mesh->GetDeformerCount(); ++i)
	{
		FbxDeformer* deformer = mesh->GetDeformer(i);

		if (deformer == NULL)
			continue;

		// Deformer�� Ÿ���� Skin�� ���, �� Skinned Mesh�� ���
		if (deformer->GetDeformerType() == FbxDeformer::eSkin)
		{
			
			FbxSkin* skin = (FbxSkin *)deformer;

			if (skin == NULL)
				continue;

			ProcessBoneWeights(skin, meshBoneWeights);
		}
	}
}

/****************************************************************************************************
 @brief
 Skin ������ �ε��Ͽ�, Mesh�� �ش� Vertex�� �� Bone�� ���Ͽ� ��ŭ�� ������ �޴���(Weight)��
 ConverterBoneWeights Class�� Vector�� ����(ũ��: VertexCount, Index: VertexIndex)
****************************************************************************************************/
void ConverterScene::ProcessBoneWeights(FbxSkin * skin, vector<ConverterBoneWeights>& meshBoneWeights)
{
	FbxCluster::ELinkMode linkMode = FbxCluster::eNormalize;

	int clusterCount = skin->GetClusterCount();
	vector<ConverterBoneWeights> skinBoneWeights(meshBoneWeights.size(), ConverterBoneWeights());
	for (int i = 0; i < clusterCount; i++)
	{
		FbxCluster* cluster = skin->GetCluster(i);
		if (cluster == NULL)
			continue;

		linkMode = cluster->GetLinkMode();
		FbxNode* pLinkNode = cluster->GetLink();

		if (pLinkNode == NULL)
			continue;

		wstring lineNodeName = String::StringToWString(pLinkNode->GetName());
		int nBoneIndex = skeleton->FindBoneIndex(lineNodeName);
		if (nBoneIndex < 0)
			continue;

		ConverterSkeletonBone* skeletonBone = skeleton->GetSkeletonBone(nBoneIndex);

		FbxAMatrix matClusterTransformMatrix;
		FbxAMatrix matClusterLinkTransformMatrix;
		cluster->GetTransformMatrix(matClusterTransformMatrix); // �Ž��� Transform Matrix
		cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix); // ���� Transform Matrix

																		//TODO: üũ
		skeletonBone->SetBindPoseTransform(ConverterUtility::ToMatrix(matClusterLinkTransformMatrix)); // ��->��Ʈ
		skeletonBone->SetBoneReferenceTransform(ConverterUtility::ToMatrix(matClusterTransformMatrix)); // �Ž�->��Ʈ

		int* indices = cluster->GetControlPointIndices(); //�ش� Bone�� ������ �޴� ��� Vertex Index���� �迭
		double* weights = cluster->GetControlPointWeights(); // �� Vertex Index�� ���� Weight���� �迭

															 // indices[j]�� Index�� ���� Vertex�� ������ �ִ� Bone�� Idx��, �� ��ġ�� Weights��
															 // ConverterBoneWeights Class�� �������� �߰�
		for (int j = 0; j < cluster->GetControlPointIndicesCount(); ++j)
			skinBoneWeights[indices[j]].AddBoneWeight(nBoneIndex, (float)weights[j]);
	}

	// Weight�� ���
	switch (linkMode)
	{
		case FbxCluster::eNormalize: // ��� Weight�� ���� 1 �� ����ȭ
			{
				for (int i = 0; i < (int)skinBoneWeights.size(); ++i)
					skinBoneWeights[i].Normalize();
			}
			break;

		case FbxCluster::eAdditive: // �״�� �ջ�
			break;

		case FbxCluster::eTotalOne: // ��� Weight�� ���� 1 �̾�� ��(����ȭ ���� ����)
			break;
	}

	for (size_t i = 0; i < meshBoneWeights.size(); i++)
		meshBoneWeights[i].AddBoneWeights(skinBoneWeights[i]);
}

/****************************************************************************************************
 @biref
 �ش� Vertex Index�� Material ������ ConverterMaterial Class �������� ��ȯ
****************************************************************************************************/
ConverterMaterial * ConverterScene::LinkMaterialWithPolygon(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
{
	if (layerIndex < 0 || layerIndex > mesh->GetLayerCount())
		return NULL;

	
	FbxNode* node = mesh->GetNode();
	if (node == NULL) return NULL;

	FbxLayerElementMaterial* fbxMaterial = mesh->GetLayer(layerIndex)->GetMaterials();
	if (fbxMaterial == NULL) return NULL;

	int mappingIndex = GetMappingIndex
	(
		fbxMaterial->GetMappingMode()
		, polygonIndex
		, 0
		, vertexIndex
	);
	if (mappingIndex < 0) return NULL;

	// Material�� Vertex Index�� ����� ��带 �ε��� ��,
	// �̿� �µ��� mappingIndex�� ��� ��, GetMaterial�� �ش� Vertex�� Material �� ��ȯ
	FbxLayerElement::EReferenceMode refMode = fbxMaterial->GetReferenceMode();
	if (refMode == FbxLayerElement::eDirect)
	{
		if (mappingIndex < node->GetMaterialCount())
			return GetMaterial(node->GetMaterial(mappingIndex));
	}
	else if (refMode == FbxLayerElement::eIndexToDirect)
	{
		FbxLayerElementArrayTemplate<int>& indexArr = fbxMaterial->GetIndexArray();

		if (mappingIndex < indexArr.GetCount())
		{
			int tempIndex = indexArr.GetAt(mappingIndex);

			if (tempIndex < node->GetMaterialCount())
				return GetMaterial(node->GetMaterial(tempIndex));
		}//if(mappingIndex)
	}//if(refMode)

	return NULL;
}

/****************************************************************************************************
@biref
MappingMode�� ���� Index�� ����Ͽ� ��ȯ
****************************************************************************************************/
int ConverterScene::GetMappingIndex(FbxLayerElement::EMappingMode mode, int polygonIndex, int polygonVertexIndex, int vertexIndex)
{
	switch (mode)
	{
		case FbxLayerElement::eAllSame: 
			return 0;
		case FbxLayerElement::eByControlPoint: 
			return vertexIndex;
		case FbxLayerElement::eByPolygonVertex: 
			return polygonIndex * 3 + polygonVertexIndex;
		case FbxLayerElement::eByPolygon:
			return polygonIndex;
	}
	
	return -1;
}

/****************************************************************************************************
@biref
�ش� Vertex Index�� UV���� �ε��Ͽ� ��ȯ
****************************************************************************************************/
D3DXVECTOR2 ConverterScene::GetUV(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
{
	int layerCount = mesh->GetLayerCount();
	if (layerIndex >= layerCount) return D3DXVECTOR2();

	FbxLayer* layer = mesh->GetLayer(layerIndex);
	if(layer == NULL) return D3DXVECTOR2();

	// Diffuse Texture�� Uv�ε�
	FbxLayerElementUV* uv = layer->GetUVs(FbxLayerElement::eTextureDiffuse);
	if(uv == NULL) return D3DXVECTOR2();


	FbxLayerElement::EMappingMode mappingMode = uv->GetMappingMode();
	FbxLayerElement::EReferenceMode refMode = uv->GetReferenceMode();

	const FbxLayerElementArrayTemplate<FbxVector2>& uvArray = uv->GetDirectArray();
	const FbxLayerElementArrayTemplate<int>& uvIndexArray = uv->GetIndexArray();

	int mappingIndex = 0;
	if (mappingMode == FbxLayerElement::eByControlPoint)
	{
		mappingIndex = vertexIndex;

		if (refMode == FbxLayerElement::eDirect)
		{
			if(mappingIndex < uvArray.GetCount())
				return ConverterUtility::ToUv(uvArray.GetAt(mappingIndex));
		}
		else if (refMode == FbxLayerElement::eIndexToDirect)
		{
			if (mappingIndex < uvArray.GetCount())
			{
				int tempIndex = uvIndexArray.GetAt(mappingIndex);

				if(tempIndex < uvIndexArray.GetCount())
					return ConverterUtility::ToUv(uvArray.GetAt(tempIndex));
			}
		}//if(refMode)
	}
	else if (mappingMode == FbxLayerElement::eByPolygonVertex)
	{
		mappingIndex = mesh->GetTextureUVIndex
		(
			polygonIndex
			, polygonVertexIndex
			, FbxLayerElement::eTextureDiffuse
		);

		switch (refMode)
		{
			case FbxLayerElement::eDirect:
			case FbxLayerElement::eIndexToDirect:
				if (mappingIndex < uvArray.GetCount())
					return ConverterUtility::ToUv(uvArray.GetAt(mappingIndex));
		}//switch(refMode)
	}//if(mappingMode)

	return D3DXVECTOR2();
}

/****************************************************************************************************
 @biref
 FbxSurfaceMaterial ���� ����� ���� ConverterMaterial Class�� ��ȯ
****************************************************************************************************/
ConverterMaterial * ConverterScene::GetMaterial(FbxSurfaceMaterial * fbxMaterial)
{
	for (size_t i = 0; i < fbxMaterials.size(); i++)
	{
		if (fbxMaterials[i] == fbxMaterial)
			return materials[i];
	}

	return NULL;
}

/****************************************************************************************************
 @biref
 Node�� Offset Transform Matrix�� ��ȯ
****************************************************************************************************/
D3DXMATRIX ConverterScene::GetGeometricOffset(FbxNode * node)
{
	if (node == NULL)
	{
		D3DXMATRIX matrix;
		D3DXMatrixIdentity(&matrix);

		return matrix;
	}

	FbxVector4 T = node->GetGeometricTranslation(FbxNode::eSourcePivot);
	FbxVector4 R = node->GetGeometricRotation(FbxNode::eSourcePivot);
	FbxVector4 S = node->GetGeometricScaling(FbxNode::eSourcePivot);

	FbxAMatrix offset = FbxAMatrix(T, R, S);

	return ConverterUtility::ToMatrix(offset);
}

/****************************************************************************************************
 @biref
 ������ time���� node�� Global Transform Matrix(���->��Ʈ)�� �ε��Ͽ� ��ȯ
****************************************************************************************************/
D3DXMATRIX ConverterScene::GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if (node == NULL)
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}
	
	FbxAMatrix matrix = node->EvaluateGlobalTransform(time);
	
	return ConverterUtility::ToMatrix(matrix);
}

/****************************************************************************************************
 @biref
 ������ time���� node�� Local Transform Matrix(�ڽ�->�θ�)�� �ε��Ͽ� ��ȯ
****************************************************************************************************/
D3DXMATRIX ConverterScene::GetLocalTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if( node == NULL )
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}

	FbxAMatrix matrix = node->EvaluateLocalTransform(time);

	return ConverterUtility::ToMatrix(matrix);
}

void ConverterScene::AddAnimation(ConverterAnimation * animation)
{
	wstring animName = animation->GetName();
	if (animations.count(animName) > 0)
		return;
	else
		animations.insert({ animName, animation });
}


// Binary Export ����
void ConverterScene::ExportMaterials(wstring fileName)
{
	BinaryWriter bMaterials = BinaryWriter();
	bMaterials.Open(fileName);
	{
		bMaterials.Write(materials);
	}
	bMaterials.Close();
}

void ConverterScene::ExportSkeleton(wstring fileName)
{
	BinaryWriter bSkeleton = BinaryWriter();
	bSkeleton.Open(fileName);
	{
		bSkeleton.Write(skeleton);
	}
	bSkeleton.Close();
}

void ConverterScene::ExportConverters(wstring fileName)
{
	BinaryWriter bConverters = BinaryWriter();
	bConverters.Open(fileName);
	{
		bConverters.Write(Converters);
	}
	bConverters.Close();
}

void ConverterScene::ExportAnimations(wstring fileName)
{
	BinaryWriter bAnimations = BinaryWriter();
	bAnimations.Open(fileName);
	{
		bAnimations.Write(animations);
	}
	bAnimations.Close();
}

void ConverterScene::ImportMaterials(wstring fileName)
{
	BinaryReader bMaterials = BinaryReader();
	bMaterials.Open(fileName);
	{
		UINT materialsCount = bMaterials.UInt();
		materials.clear();
		materials.reserve(materialsCount);
		for (size_t i = 0; i < materialsCount; i++)
		{
			ConverterMaterial* material = new ConverterMaterial();
			material->Import(&bMaterials);
			materials.push_back(material);
		}
	}
	bMaterials.Close();
}

void ConverterScene::ImportSkeleton(wstring fileName)
{
	BinaryReader bSkeleton = BinaryReader();
	bSkeleton.Open(fileName);
	{
		skeleton = new ConverterSkeleton();
		skeleton->Import(&bSkeleton);
	}
	bSkeleton.Close();
}

void ConverterScene::ImportConverters(wstring fileName)
{
	BinaryReader bConverters = BinaryReader();
	bConverters.Open(fileName);
	{
		UINT ConvertersCount = bConverters.UInt();
		Converters.clear();
		Converters.reserve(ConvertersCount);
		for (size_t i = 0; i < ConvertersCount; i++)
		{
			Converter* Converter = new Converter(this);
			Converter->Import(&bConverters);
			Converters.push_back(Converter);
		}
	}
	bConverters.Close();
}

void ConverterScene::ImportAnimations(wstring fileName)
{
	BinaryReader bAnimations = BinaryReader();
	bAnimations.Open(fileName);
	{
		animations.clear();
		UINT animationKeyFramesCount = bAnimations.UInt();
		
		if (animationKeyFramesCount > 0 && animationController == NULL)
			animationController = new ConverterAnimationController();

		for (size_t i = 0; i < animationKeyFramesCount; i++)
		{
			wstring name = bAnimations.WString();
			ConverterAnimation* ConverterAnimation = new ConverterAnimation();
			ConverterAnimation->Import(&bAnimations);
			animations.insert(make_pair(name, ConverterAnimation));

			animationController->AddAnimation(ConverterAnimation);
		}

		animationController->SetCurrentAnimation(animationKeyFramesCount-1);
		animationController->Play();
	}
	bAnimations.Close();
}
