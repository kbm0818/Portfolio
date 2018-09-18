#include "stdafx.h"
#include "Editor.h"
#include "./Cameras/CameraFactory.h"
#include "./Cameras/FirstPerson.h"
#include "./Environment/Sky.h"
#include "./Meshes/Plane.h"
#include "./FbxModel/MoLoader.h"
#include "./Model/ModelAnimationController.h"

Editor* Editor::instance = NULL;

Editor::Editor()
	: model(NULL)
{
	D3DDesc desc;
	D3D::GetDesc(&desc);

	camera = CameraFactory::Create<FirstPerson>();
	camera->SetPerspective(desc.width, desc.height, (float)D3DX_PI * 0.25f, 0.1f, 1000.0f);
	camera->SetPosition(0, 5, -20);

	sky = new Sky();
	plane = new Plane();
	
//	MoLoader::Convert("C:/Users/Me/Documents/Visual Studio 2015/Projects/_Contents/Models/Paladin.fbx", "C:/Users/Me/Documents/Visual Studio 2015/Projects/_Contents/BinModels/Paladin.bin");
	
	//MoLoader::LoadBinary("C:/Users/Me/Documents/Visual Studio 2015/Projects/_Contents/BinModels/Paladin.bin", &model);
	//MoLoader::AddAnimation("C:/Users/Me/Documents/Visual Studio 2015/Projects/_Contents/Animations/Idle.fbx", &model, "idle");

//	ModelAnimationController* controller = model->GetAnimationController();
	//controller->SetCurrentAnimation(L"idle");
}

Editor::~Editor()
{
	SAFE_DELETE(model);

	SAFE_DELETE(plane);
	SAFE_DELETE(sky);
	CameraFactory::Delete(camera);
}

void Editor::BeginUpdate()
{

}

void Editor::Update()
{
	sky->Update(camera);

	if(model != NULL)
		model->Update();
}

void Editor::EndUpdate()
{
	if (Keyboard::Get()->Press('W'))
		camera->MoveForward();
	else if (Keyboard::Get()->Press('S'))
		camera->MoveBackward();

	if (Keyboard::Get()->Press('A'))
		camera->MoveLeft();
	else if (Keyboard::Get()->Press('D'))
		camera->MoveRight();

	if (Keyboard::Get()->Press('Q'))
		camera->MoveDown();
	else if (Keyboard::Get()->Press('E'))
		camera->MoveUp();

	if (Mouse::Get()->Press(1))
	{
		D3DXVECTOR3 move = Mouse::Get()->GetMoveValue();

		camera->Rotate(D3DXVECTOR2(move.y, move.x));
	}
}

void Editor::PreRender()
{
}

void Editor::Render()
{
	sky->Render(camera);
	plane->Render(camera);

	if (model != NULL)
		model->Render(camera);
}

void Editor::PostRender()
{

}

void Editor::SetModel(Model * model)
{
	SAFE_DELETE(this->model);

	this->model = model;
}


////////////////////////////////////////////////////

DLL void Cs_SetShader(const char* file)
{
	wstring path = CA2CT(file);

	Editor::Get()->GetSky()->SetShader(path);
}

DLL void Cs_FbxConvert(const char* fbxFile, const char* saveFile)
{
	MoLoader::Convert(fbxFile, saveFile);
}

DLL void Cs_BinModelLoad(const char* file)
{
	Model* model = NULL;
	MoLoader::LoadBinary(file, &model);

	Editor::Get()->SetModel(model);
}

DLL void Cs_BinModelSave(const char* file)
{
	Model* model = Editor::Get()->GetModel();
	MoLoader::WriteBinary(file, model);
}

DLL void Cs_AnimationAdd(const char* file, const char* animName)
{
	Model* model = Editor::Get()->GetModel();

	MoLoader::AddAnimation(file, &model, animName);
}

DLL void Cs_SetAnimation(const char* file)
{
	Model* model = Editor::Get()->GetModel();

	ModelAnimationController* controller = model->GetAnimationController();
	controller->SetCurrentAnimation(String::StringToWString(file));
}

DLL void Cs_PartAdd(const char* file, const char* boneName)
{
	Model* model = Editor::Get()->GetModel();

	MoLoader::AddModel(file, &model, boneName);
}


////////////////////////////////////////////////////

void Editor::Create()
{
	assert(instance == NULL);

	instance = new Editor();
}

void Editor::Delete()
{
	SAFE_DELETE(instance);
}

Editor * Editor::Get()
{
	assert(instance != NULL);

	return instance;
}