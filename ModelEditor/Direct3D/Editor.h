#pragma once

class Sky;
class Plane;
class Editor
{
public:
	static void Create();
	static void Delete();
	static Editor* Get();

	Sky* GetSky() { return sky; }
	Camera* GetCamera() { return camera; }

	Model* GetModel() { return model; }
	void SetModel(Model* model);
	
private:
	friend class Program;
	static Editor* instance;

	Sky* sky;
	Plane* plane;
	Camera* camera;

	Model* model;


	Editor();
	~Editor();

	void BeginUpdate();
	void Update();
	void EndUpdate();

	void PreRender();
	void Render();
	void PostRender();
};