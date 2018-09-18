#include "stdafx.h"
#include "FbxLoader.h"
#include "FbxDataType.h"
#include "Object/Model/Model.h"

#include "Object/Model/ModelPart.h"
#include "Object/Model/ModelAnimationController.h"
#include "Object/Model/ModelAnimation.h"
#include "Object/Model/ModelSkeleton.h"
#include "Object/Model/ModelSkeletonBone.h"
#include "Object/Model/ModelAnimationKeyFrames.h"

void FbxLoader::Convert(wstring fbxFile, wstring saveFile)
{
	FbxModelData* modelData = NULL;
	FbxLoader* loader = new FbxLoader();
	modelData = loader->Load(fbxFile);
	SAFE_DELETE(loader);

	if (wcscmp(saveFile.c_str(), L"") == 0)
	{
		wstring name;
		String::SplitFileNameOnly(fbxFile, &name);

		modelData->Write(name);
	}
	else
	{
		modelData->Write(saveFile);
	}
	
	SAFE_DELETE(modelData);
}

void FbxLoader::ConvertAnim(wstring fbxFile)
{
	string fbxPath = String::WStringToString(fbxFile);
	string animName;
	String::SplitFileNameOnly(fbxPath, &animName);

	BinaryWriter* w = new BinaryWriter;
	w->Open(Contents + L"UserBinaryAnim/" + String::StringToWString(animName) + L".ani");
	FbxLoader* loader = new FbxLoader();

	loader->LoadAnim(w, fbxPath, animName);

	w->Close();
	SAFE_DELETE(loader);
	SAFE_DELETE(w);
}

IMeshData * FbxLoader::Load(wstring modelName, GlobalValues * values)
{
	Model* model = new Model;
	model->Load(modelName, values);
	 
	return model;
}

bool FbxLoader::LoadAnim(wstring animName, Model* model)
{
	if (model->skeleton == nullptr)
		return false;

	if (model->animationController == nullptr)
		model->animationController = new ModelAnimationController;

	BinaryReader* r = new BinaryReader;
	r->Open(Contents + L"UserBinaryAnim/" + animName + L".ani");

	wstring animNameG = r->Wstring();
	int keyFrameG = r->Int();
	float rateG = r->Float();
	float defaultRateG = r->Float();

	ModelAnimation* anim = new ModelAnimation(animNameG, keyFrameG, rateG, defaultRateG);
	model->animationController->AddAnimation(anim);

	int count = r->Int();
	for (int i = 0; i < count; i++)
	{
		wstring boneName = r->Wstring();
		ModelAnimationKeyFrames* akf = new ModelAnimationKeyFrames();
		akf->Load(r);

		ModelSkeletonBone* bone = model->skeleton->FindBone(boneName);
		
		if (bone == nullptr)
		{
			r->Close();
			SAFE_DELETE(r);
			return false;
		}

		bone->AddAnimationKeyFrames(akf);
	}

	r->Close();
	SAFE_DELETE(r);

	return true;
}

FbxLoader::FbxLoader()
{
	manager = FbxManager::Create();
	assert(manager != NULL);

	scene = FbxScene::Create(manager, "");
	assert(scene != NULL);
}

FbxLoader::~FbxLoader()
{
	scene->Destroy();
	manager->Destroy();
}

FbxModelData * FbxLoader::Load(string file)
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

	FbxSystemUnit sceneSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
	if (sceneSystemUnit != FbxSystemUnit::m) {
		FbxSystemUnit::m.ConvertScene(scene);
	}

	converter = new FbxGeometryConverter(manager);
	{
		model = new FbxModelData(file);

		model->skeleton = new FbxSkeletonData();

		ProcessMaterial();
		ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eSkeleton); // 스켈레톤
		ProcessNode(scene->GetRootNode(), FbxNodeAttribute::eMesh);
	}
	SAFE_DELETE(converter);

	ios->Destroy();
	importer->Destroy();

	return model;
}

FbxModelData * FbxLoader::Load(wstring file)
{
	Load(String::WStringToString(file));

	return model;
}

void FbxLoader::LoadAnim(BinaryWriter* w, string file, string animName)
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

	FbxSystemUnit sceneSystemUnit = scene->GetGlobalSettings().GetSystemUnit();
	if (sceneSystemUnit != FbxSystemUnit::m) {
		FbxSystemUnit::m.ConvertScene(scene);
	}

	converter = new FbxGeometryConverter(manager);
	{
		ProcessAnimations(w, animName);
	}
	SAFE_DELETE(converter);

	ios->Destroy();
	importer->Destroy();
}

void FbxLoader::ProcessMaterial()
{
	for (int i = 0; i < scene->GetMaterialCount(); i++)
	{
		FbxSurfaceMaterial* temp = scene->GetMaterial(i);
		materials.push_back(temp);

		FbxMaterialData* material = new FbxMaterialData(i, temp);
		model->PushMaterial(material);
	}
}

void FbxLoader::ProcessNode(FbxNode * node, FbxNodeAttribute::EType type)
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

void FbxLoader::ProcessMesh(FbxNode * node)
{
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	converter->Triangulate(attribute, true, true);

	FbxMesh* mesh = node->GetMesh();
	assert(mesh != NULL);

	int vertexCount = mesh->GetControlPointsCount();
	assert(vertexCount > 0);

	vector<FbxBoneWeightsData> boneWeights(vertexCount, FbxBoneWeightsData());
	if (model->skeleton != nullptr)
	{
		ProcessBoneWeights(mesh, boneWeights);
	}

	FbxPartData* part = new FbxPartData(node->GetName());

	FbxVector4* controlPoints = mesh->GetControlPoints();
	for (int i = 0; i < mesh->GetPolygonCount(); i++)
	{
		FbxMaterialData* material = NULL;

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

			D3DXVECTOR3 vecPos = FbxModelUtility::ToVector3(fbxPos);
			D3DXVECTOR3 vecNormal = FbxModelUtility::ToVector3(fbxNormal);

			D3DXVECTOR2 vecUv = GetUV(mesh, 0, i, vi, vertexIndex);
			if (model->skeleton == NULL)
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

void FbxLoader::ProcessSkeleton(FbxNode * node)
{
	FbxSkeleton* fbxSkeleton = node->GetSkeleton();

	if (model->skeleton == NULL)
		model->skeleton = new FbxSkeletonData();

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
	FbxSkeletonBoneData* skeletonBone = new FbxSkeletonBoneData(tempName, parentBoneIndex);
	model->skeleton->AddSkeletonBone(skeletonBone);
}

void FbxLoader::ProcessBoneWeights(FbxMesh * mesh, vector<FbxBoneWeightsData>& meshBoneWeights)
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

void FbxLoader::ProcessBoneWeights(FbxSkin * skin, vector<FbxBoneWeightsData>& meshBoneWeights)
{
	FbxCluster::ELinkMode linkMode = FbxCluster::eNormalize;

	int clusterCount = skin->GetClusterCount();
	vector<FbxBoneWeightsData> skinBoneWeights(meshBoneWeights.size(), FbxBoneWeightsData());
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

		FbxSkeletonBoneData* skeletonBone = model->skeleton->GetSkeletonBone(nBoneIndex);

		FbxAMatrix matClusterTransformMatrix;
		FbxAMatrix matClusterLinkTransformMatrix;
		cluster->GetTransformMatrix(matClusterTransformMatrix);
		cluster->GetTransformLinkMatrix(matClusterLinkTransformMatrix);

		//TODO: 체크
		skeletonBone->SetBindPoseTransform(FbxModelUtility::ToMatrix(matClusterLinkTransformMatrix));
		skeletonBone->SetBoneReferenceTransform(FbxModelUtility::ToMatrix(matClusterTransformMatrix));

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

void FbxLoader::ProcessAnimations(BinaryWriter* w, string animName)
{
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

			FbxAnimationData* animation = new FbxAnimationData(String::StringToWString(animName), keyFrames, frameRate);
			animation->Write(w);
			w->Int(rootNode->GetChildCount(true));
			ProcessAnimation(w, rootNode, String::StringToWString(animName), frameRate, (float)startTime, (float)endTime);
		}
	}
	takeArray.Clear();
}

void FbxLoader::ProcessAnimation(BinaryWriter* w, FbxNode * node, wstring takeName, float frameRate, float start, float stop)
{
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	if (nodeAttribute != NULL)
	{
		if (nodeAttribute->GetAttributeType() == FbxNodeAttribute::eSkeleton)
		{
			wstring boneName = String::StringToWString(node->GetName());
			FbxAnimationKeyFrameDatas* animationKeyFrames = new FbxAnimationKeyFrameDatas(takeName);

			double time = 0;
			while (time <= (double)stop)
			{
				FbxTime takeTime;
				takeTime.SetSecondDouble(time);
				
				D3DXMATRIX matAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node, takeTime);
				D3DXMATRIX matParentAbsoluteTransform = GetAbsoluteTransformFromCurrentTake(node->GetParent(), takeTime);

				D3DXMATRIX matInvParentAbsoluteTransform;
				D3DXMatrixInverse(&matInvParentAbsoluteTransform, NULL, &matParentAbsoluteTransform);

				D3DXMATRIX matTransform = matAbsoluteTransform * matInvParentAbsoluteTransform;
				animationKeyFrames->AddKeyFrame(matTransform);

				D3DXMATRIX localTransform = GetLocalTransformFromCurrentTake(node, takeTime);
				animationKeyFrames->AddKeyFrame(localTransform);

				time += (double)(1.0f / frameRate);
			}

			w->WString(boneName);
			animationKeyFrames->Write(w);
		}
	}//if(nodeAttribute)

	for (int i = 0; i < node->GetChildCount(); ++i)
		ProcessAnimation(w, node->GetChild(i), takeName, frameRate, start, stop);
}

D3DXMATRIX FbxLoader::GetAbsoluteTransformFromCurrentTake(FbxNode * node, FbxTime time)
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

	return FbxModelUtility::ToMatrix(matrix);
}

D3DXMATRIX FbxLoader::GetLocalTransformFromCurrentTake(FbxNode * node, FbxTime time)
{
	if (node == NULL)
	{
		D3DXMATRIX mat;
		D3DXMatrixIdentity(&mat);

		return mat;
	}

	FbxAMatrix matrix = node->EvaluateLocalTransform(time);

	return FbxModelUtility::ToMatrix(matrix);
}

FbxMaterialData * FbxLoader::LinkMaterialWithPolygon(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
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

int FbxLoader::GetMappingIndex(FbxLayerElement::EMappingMode mode, int polygonIndex, int polygonVertexIndex, int vertexIndex)
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

D3DXVECTOR2 FbxLoader::GetUV(FbxMesh * mesh, int layerIndex, int polygonIndex, int polygonVertexIndex, int vertexIndex)
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
				return FbxModelUtility::ToUv(uvArray.GetAt(mappingIndex));
		}
		else if (refMode == FbxLayerElement::eIndexToDirect)
		{
			if (mappingIndex < uvArray.GetCount())
			{
				int tempIndex = uvIndexArray.GetAt(mappingIndex);

				if (tempIndex < uvArray.GetCount())
					return FbxModelUtility::ToUv(uvArray.GetAt(tempIndex));
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
				return FbxModelUtility::ToUv(uvArray.GetAt(mappingIndex));
		}//switch(refMode)
	}//if(mappingMode)

	return D3DXVECTOR2();
}

FbxMaterialData * FbxLoader::GetMaterial(FbxSurfaceMaterial * fbxMaterial)
{
	for (size_t i = 0; i < materials.size(); i++)
	{
		if (materials[i] == fbxMaterial)
			return model->materials[i];
	}

	return NULL;
}

D3DXMATRIX FbxLoader::GetGeometricOffset(FbxNode * node)
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

	return FbxModelUtility::ToMatrix(offset);
}
