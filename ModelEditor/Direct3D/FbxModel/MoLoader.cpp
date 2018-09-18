#include "../stdafx.h"
#include "MoLoader.h"
#include "MoModel.h"
#include "MoMaterial.h"
#include "MoModelUtility.h"
#include "MoSkeleton.h"
#include "MoSkeletonBone.h"
#include "MoBoneWeights.h"
#include "../Model/Model.h"
#include "../Model/ModelAnimationController.h"
#include "../Model/ModelAnimation.h"
#include "../Model/ModelAnimationKeyFrames.h"
#include "../Model/ModelSkeleton.h"
#include "../Model/ModelSkeletonBone.h"
#include "../Model/ModelMesh.h"
#include "../Model/ModelPart.h"
#include "MoPart.h"

void MoLoader::Convert(string fbxFile, string saveFile)
{
	MoModel* model = NULL;
	MoLoader* loader = new MoLoader();
	model = loader->Load(fbxFile);
	SAFE_DELETE(loader);

	model->Write(saveFile);
	SAFE_DELETE(model);
}

void MoLoader::LoadBinary(string binFile, Model ** model)
{
	MoModel::Read(binFile, model);
}

void MoLoader::WriteBinary(string binFile, Model * model)
{
	MoModel::Write(binFile, model);
}

void MoLoader::AddAnimation(string fbxAnimFile, Model ** model, string animName)
{
	string temp = animName;
	if (strcmp(temp.c_str(), "") == 0)
	{
		String::SplitFilePath(fbxAnimFile, NULL, &temp);
		String::DeleteExt(&temp, temp);
	}

	MoLoader* loader = new MoLoader();
	loader->LoadAnim(fbxAnimFile, model, temp);
	SAFE_DELETE(loader);
}

void MoLoader::AddModel(string binFile, Model ** model, string boneName)
{
	UINT boneIndex = (*model)->skeleton->FindBoneIndex(String::StringToWString(boneName));

	if (boneIndex < 0)
		return;

	MoLoader* loader = new MoLoader();
	loader->LoadPart(binFile, model);
	SAFE_DELETE(loader);

	UINT size = (*model)->parts.size();
	(*model)->parts[size - 1]->SetEquip(true);

	ModelSkeletonBone* bone = (*model)->skeleton->GetSkeletonBone(boneIndex);

	(*model)->parts[size - 1]->SetModelSkeletonBone(bone);
	(*model)->parts[size - 1]->boneName = String::StringToWString(boneName);

	ModelMaterial* material = (*model)->GetMatchMaterial((*model)->materials.size() - 1); // 마지막 메테리얼을 가져와서 // 아마 마지막에 저장되있을꺼니
																							//안되있으면 전에꺼가 되겠지
	for each(ModelMesh* mesh in (*model)->parts[size - 1]->meshes)
	{
		mesh->SetMaterial(material);
	}
}

MoLoader::MoLoader()
{
	manager = FbxManager::Create();
	assert(manager != NULL);

	scene = FbxScene::Create(manager, "");
	assert(scene != NULL);
}

MoLoader::~MoLoader()
{
	scene->Destroy();
	manager->Destroy();
}

MoModel * MoLoader::Load(string file)
{
	int major, minor, revision;
	FbxManager::GetFileFormatVersion(major, minor, revision);

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	manager->SetIOSettings(ios);

	importer = FbxImporter::Create(manager, "");
	bool check = importer->Initialize(file.c_str(), -1, ios);
	assert(check == true);

	check = importer->Import(scene);
	assert(check == true);

	//CHECK
	if (scene->GetGlobalSettings().GetSystemUnit() == FbxSystemUnit::cm)
	{
		const FbxSystemUnit::ConversionOptions lConversionOptions = {
			false, /* mConvertRrsNodes */
			true, /* mConvertAllLimits */
			true, /* mConvertClusters */
			true, /* mConvertLightIntensity */
			true, /* mConvertPhotometricLProperties */
			true  /* mConvertCameraClipPlanes */
		};

		// Convert the scene to meters using the defined options.
		FbxSystemUnit::m.ConvertScene(scene, lConversionOptions);
	}

	converter = new FbxGeometryConverter(manager);
	{
		model = new MoModel(file);

		model->skeleton = new MoSkeleton();
	
		ProcessMaterial();
		ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eSkeleton); // 스켈레톤
		ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eMesh);
	}

	SAFE_DELETE(converter);

	ios->Destroy();
	importer->Destroy();

	return model;
}

void MoLoader::LoadAnim(string file, Model** model, string animName)
{
	int major, minor, revision;
	FbxManager::GetFileFormatVersion(major, minor, revision);

	FbxIOSettings* ios = FbxIOSettings::Create(manager, IOSROOT);
	ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	manager->SetIOSettings(ios);

	importer = FbxImporter::Create(manager, "");
	bool check = importer->Initialize(file.c_str(), -1, ios);
	assert(check == true);

	check = importer->Import(scene);
	assert(check == true);

	if (scene->GetGlobalSettings().GetSystemUnit() == FbxSystemUnit::cm)
	{
		const FbxSystemUnit::ConversionOptions lConversionOptions = {
			false, /* mConvertRrsNodes */
			true, /* mConvertAllLimits */
			true, /* mConvertClusters */
			true, /* mConvertLightIntensity */
			true, /* mConvertPhotometricLProperties */
			true  /* mConvertCameraClipPlanes */
		};

		// Convert the scene to meters using the defined options.
		FbxSystemUnit::m.ConvertScene(scene, lConversionOptions);
	}

	converter = new FbxGeometryConverter(manager);
	{
		ProcessAnimations(model, animName);
	}
	SAFE_DELETE(converter);

	ios->Destroy();
	importer->Destroy();
}

void MoLoader::LoadPart(string file, Model ** model)
{
	string tempLocation;
	String::DeleteExt(&tempLocation, file);
	tempLocation += ".tmp";
	Convert(file, tempLocation);

	MoModel::ReadParts(tempLocation, model);
}

void MoLoader::ProcessAnimations(Model** model,string animName)
{
	if ((*model)->animationController == NULL)
		(*model)->animationController = new ModelAnimationController();

	FbxNode* rootNode = scene->GetRootNode();
	if (rootNode == NULL) return;

	string test = rootNode->GetName();
	float frameRate = (float)FbxTime::GetFrameRate(scene->GetGlobalSettings().GetTimeMode());

	FbxArray<FbxString *> takeArray;
	FbxDocument* document = dynamic_cast<FbxDocument *>(scene);
	if (document != NULL)
		document->FillAnimStackNameArray(takeArray);

	for (int i = 0; i < importer->GetAnimStackCount(); i++)
	{
		FbxTakeInfo* takeInfo = importer->GetTakeInfo(i);
		//wstring takeName = String::StringToWString(takeInfo->mName.Buffer());

		FbxTime start = FbxTime(FbxLongLong(0x7fffffffffffffff));
		FbxTime stop = FbxTime(FbxLongLong(-0x7fffffffffffffff));

		FbxTimeSpan span = takeInfo->mLocalTimeSpan;

		double startTime = span.GetStart().GetSecondDouble();
		double endTime = span.GetStop().GetSecondDouble(); 

		if (startTime < endTime)
		{
			// KeyFrames의 수 = Animation 실행 시간(초) * 초당 Frmae 수
			int keyFrames = (int)((endTime - startTime) * (double)frameRate);

			ModelAnimation* animation = new ModelAnimation(String::StringToWString(animName), keyFrames, frameRate);
			((*model)->animationController)->AddAnimation(animation);

			ProcessAnimation(model, rootNode, String::StringToWString(animName), frameRate, (float)startTime, (float)endTime);
		}
	}
	takeArray.Clear();

	//TODO: defualt 스택 세팅
}

void MoLoader::ProcessAnimation(Model** model, FbxNode * node, wstring takeName, float frameRate, float start, float stop)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != NULL)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			if ((*model)->skeleton != NULL)
			{
				wstring temp = String::StringToWString(node->GetName());
				ModelSkeletonBone* bone = (*model)->skeleton->FindBone(temp);

				if (bone != NULL)
				{
					ModelAnimationKeyFrames* animationKeyFrames = new ModelAnimationKeyFrames(takeName);

					double time = 0;
					while (time <= (double)stop)
					{
						FbxTime takeTime;
						takeTime.SetSecondDouble(time);

					/*	D3DXMATRIX matAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node, takeTime);
						D3DXMATRIX matParentAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node->GetParent(), takeTime);

						D3DXMATRIX matInvParentAbsoluteTransform;
						D3DXMatrixInverse(&matInvParentAbsoluteTransform, NULL, &matParentAbsoluteTransform);

						D3DXMATRIX matTransform = matAbsoluteTransform * matInvParentAbsoluteTransform;
						animationKeyFrames->AddKeyFrame(matTransform);*/

						D3DXMATRIX localTransform = GetLocalTransformFromCurrentTake(node, takeTime);
						animationKeyFrames->AddKeyFrame(localTransform);

						time += 1.0f / frameRate;
					}

					bone->AddAnimationKeyFrames(animationKeyFrames);
				}//if(bone)
			}//if(skeleton)
		}
		else if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			ModelPart* part = NULL;
			for each(ModelPart* temp in (*model)->parts)
			{
				string nodeName = node->GetName();
				if (nodeName == temp->GetName())
				{
					part = temp;

					break;
				}
			}

			if (part != NULL)
			{
				ModelAnimationKeyFrames* animationKeyFrames = new ModelAnimationKeyFrames(takeName);

				double time = 0;
				while (time <= (double)stop)
				{
					FbxTime takeTime;
					takeTime.SetSecondDouble(time);

					D3DXMATRIX matAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node, takeTime);
					animationKeyFrames->AddKeyFrame(matAbsoluteTransform);

					time += 1.0f / frameRate;
				}

				//mesh->AddAnimationKeyFrames(animationKeyFrames);
			}
		}//if(nodeAttribute->GetAttributeType())
	}//if(nodeAttribute)

	for (int i = 0; i < node->GetChildCount(); ++i)
		ProcessAnimation(model , node->GetChild(i), takeName, frameRate, start, stop);
}

D3DXMATRIX MoLoader::GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if (node == NULL)
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}

	/*FbxAnimEvaluator* e = node->GetAnimationEvaluator();
	FbxAMatrix matrix = e->GetNodeGlobalTransform(node, time, FbxNode::eSourcePivot, false);*/
	FbxAMatrix matrix = node->EvaluateGlobalTransform(time);

	return MoModelUtility::ToMatrix(matrix);
}

void MoLoader::ProcessMaterial()
{
	for (int i = 0; i < scene->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* temp = scene->GetMaterial(i);
		materials.push_back(temp);

		MoMaterial* material = new MoMaterial(i, temp);
		model->PushMaterial(material);
	}
}

void MoLoader::ProcessNode(FbxNode * node, FbxNodeAttribute::EType type)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (attribute != NULL)
	{
		FbxNodeAttribute::EType nodeType = attribute->GetAttributeType();
		if (nodeType == type)
		{
			switch (nodeType)
			{
				case FbxNodeAttribute::eSkeleton:
					ProcessSkeleton(node);
					break;

				case FbxNodeAttribute::eMesh:
					ProcessMesh(node);
					break;

				case FbxNodeAttribute::eMarker:
				case FbxNodeAttribute::eNurbs:
				case FbxNodeAttribute::ePatch:
				case FbxNodeAttribute::eCamera:
				case FbxNodeAttribute::eLight:
				case FbxNodeAttribute::eNull:
					break;
			}//switch(nodeType)
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
		ProcessNode(node->GetChild(i), type);
}

void MoLoader::ProcessMesh(FbxNode * node)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	converter->Triangulate(attribute, true, true);

	FbxMesh* mesh = node->GetMesh();
	assert(mesh != NULL);

	int vertexCount = mesh->GetControlPointsCount();
	assert(vertexCount > 0);

	vector<MoBoneWeights> boneWeights(vertexCount, MoBoneWeights());
	if (model->skeleton != nullptr)
	{
		ProcessBoneWeights(mesh, boneWeights);
	}

	MoPart* part = new MoPart(node->GetName());

	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		MoMaterial* material = NULL;

		for (int vi = 0; vi < 3; vi++)
		{
			int vertexIndex = mesh->GetPolygonVertex(i, vi);
			if (vertexIndex < 0 || vertexIndex >= vertexCount)
				continue;

			if (material == NULL)
				material = LinkMaterialWithPolygon(mesh, 0, i, 0, vertexIndex);

			FbxVector4 fbxPos = controlPoints[vertexIndex];
			FbxVector4 fbxNormal;
			mesh->GetPolygonVertexNormal(i, vi, fbxNormal);
			fbxNormal.Normalize();

			D3DXVECTOR3 vecPos = MoModelUtility::ToVector3(fbxPos);
			D3DXVECTOR3 vecNormal = MoModelUtility::ToVector3(fbxNormal);

			D3DXVECTOR2 vecUv = GetUV(mesh, 0, i, vi, vertexIndex);
			if(model->skeleton == NULL)
				part->AddVertex(material, vecPos, vecNormal, vecUv);
			else
				part->AddVertex(material, vecPos, vecNormal, vecUv, boneWeights[vertexIndex]);
		}//for(vi)
	}//for(i)

	D3DXMATRIX temp = GetAbsoluteTransformFromCurrentTake(node, FbxTime(0));

	part->SetAbsoluteTransform(temp);
	part->SetGeometricOffset(GetGeometricOffset(node)); // 메쉬들이 그려질 기본 정보

	model->parts.push_back(part);
}

void MoLoader::ProcessSkeleton(FbxNode * node)
{
	FbxSkeleton* fbxSkeleton = node->GetSkeleton();

	if (model->skeleton == NULL)
		model->skeleton = new MoSkeleton();

	if (fbxSkeleton == NULL)
		return;

	int parentBoneIndex = -1;
	FbxNode* parentNode = node->GetParent();
	if (parentNode != NULL)
	{
		wstring nodeName = String::StringToWString(parentNode->GetName());
		parentBoneIndex = model->skeleton->FindBoneIndex(nodeName);
	}

	wstring tempName = String::StringToWString(node->GetName());
	MoSkeletonBone* skeletonBone = new MoSkeletonBone(tempName, parentBoneIndex);
	model->skeleton->AddSkeletonBone(skeletonBone);
}

MoMaterial * MoLoader::LinkMaterialWithPolygon(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
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

int MoLoader::GetMappingIndex(FbxLayerElement::EMappingMode mode, int polygonIndex, int polygonVertexIndex, int vertexIndex)
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
		default:
			return -1;
	}
}

D3DXVECTOR2 MoLoader::GetUV(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
{
	int layerCount = mesh->GetLayerCount();
	if (layerIndex >= layerCount) return D3DXVECTOR2();

	FbxLayer* layer = mesh->GetLayer(layerIndex);
	if (layer == NULL) return D3DXVECTOR2();

	FbxLayerElementUV* uv = layer->GetUVs(FbxLayerElement::eTextureDiffuse);
	if (uv == NULL) return D3DXVECTOR2();


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
			if (mappingIndex < uvArray.GetCount())
				return MoModelUtility::ToUv(uvArray.GetAt(mappingIndex));
		}
		else if (refMode == FbxLayerElement::eIndexToDirect)
		{
			if (mappingIndex < uvArray.GetCount())
			{
				int tempIndex = uvIndexArray.GetAt(mappingIndex);

				if (tempIndex < uvArray.GetCount())
					return MoModelUtility::ToUv(uvArray.GetAt(tempIndex));
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
					return MoModelUtility::ToUv(uvArray.GetAt(mappingIndex));
		}//switch(refMode)
	}//if(mappingMode)

	return D3DXVECTOR2();
}

MoMaterial * MoLoader::GetMaterial(FbxSurfaceMaterial * fbxMaterial)
{
	for (size_t i = 0; i < materials.size(); i++)
	{
		if (materials[i] == fbxMaterial)
			return model->materials[i];			
	}

	return NULL;
}

D3DXMATRIX MoLoader::GetGeometricOffset(FbxNode * node)
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

	FbxMatrix offset;
	offset.SetIdentity();
	offset.SetTRS(T, R, S);

	return MoModelUtility::ToMatrix(offset);
}

void MoLoader::ProcessBoneWeights(FbxMesh * mesh, vector<MoBoneWeights>& meshBoneWeights)
{
	if (model->skeleton == NULL)
		return;

	for (int i = 0; i < mesh->GetDeformerCount(); ++i)
	{
		FbxDeformer* deformer = mesh->GetDeformer(i);

		if (deformer == NULL)
			continue;


		if (deformer->GetDeformerType() == FbxDeformer::eSkin)
		{

			FbxSkin* skin = (FbxSkin *)deformer;

			if (skin == NULL)
				continue;


			ProcessBoneWeights(skin, meshBoneWeights);
		}
	}
}

void MoLoader::ProcessBoneWeights(FbxSkin * skin, vector<MoBoneWeights>& meshBoneWeights)
{
	FbxCluster::ELinkMode linkMode = FbxCluster::eNormalize;

	int clusterCount = skin->GetClusterCount();
	vector<MoBoneWeights> skinBoneWeights(meshBoneWeights.size(), MoBoneWeights());
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
		int nBoneIndex = model->skeleton->FindBoneIndex(lineNodeName);
		if (nBoneIndex < 0)
			continue;

		MoSkeletonBone* skeletonBone = model->skeleton->GetSkeletonBone(nBoneIndex);

		FbxAMatrix matClusterTransformMatrix;
		FbxAMatrix matClusterLinkTransformMatrix;
		cluster->GetTransformMatrix(matClusterTransformMatrix);
		cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);

		//TODO: 체크
		skeletonBone->SetBindPoseTransform(MoModelUtility::ToMatrix(matClusterLinkTransformMatrix));
		skeletonBone->SetBoneReferenceTransform(MoModelUtility::ToMatrix(matClusterTransformMatrix));

		int* indices = cluster->GetControlPointIndices();
		double* weights = cluster->GetControlPointWeights();

		for (int j = 0; j < cluster->GetControlPointIndicesCount(); ++j)
			skinBoneWeights[indices[j]].AddBoneWeight(nBoneIndex, (float)weights[j]);
	}

	switch (linkMode)
	{
	case FbxCluster::eNormalize:
	{
		for (int i = 0; i < (int)skinBoneWeights.size(); ++i)
			skinBoneWeights[i].Normalize();
	}
	break;

	case FbxCluster::eAdditive:
		break;

	case FbxCluster::eTotalOne:
		break;
	}

	for (size_t i = 0; i < meshBoneWeights.size(); i++)
		meshBoneWeights[i].AddBoneWeights(skinBoneWeights[i]);
}
/****************************************************************************************************
@biref
임의의 time에서 node의 Local Transform Matrix(자신->부모)를 로드하여 반환
****************************************************************************************************/
D3DXMATRIX MoLoader::GetLocalTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if (node == NULL)
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}

	FbxAMatrix matrix = node->EvaluateLocalTransform(time);

	return MoModelUtility::ToMatrix(matrix);
}