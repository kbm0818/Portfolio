#pragma once

class IScene;
class World;
class SceneManager
{
public:
	enum SceneMessage { PUSH, POP };

	static SceneManager* Get();
	static void Create();
	static void Delete();

	void Update();
	void LateUpdate();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

	void PushScene(IScene* scene);
	void PopScene();
	void ChangeScene(IScene* scene);
	IScene* GetCurrentScene() { return currentScene; }
	World* GetCurrentWorld() { return currentWorld; }
	void GameExit() { exit(1); }

	D3DXVECTOR2 GetScreenSize() { return D3DXVECTOR2(screenWidth, screenHeight); }
private:
	typedef struct tagSceneMessage
	{
		SceneMessage kind;
		IScene*		 scene;
		tagSceneMessage() {}
		tagSceneMessage(SceneMessage k, IScene* s)
		{
			kind = k;
			scene = s;
		}
	}SCENE_MSG;

	SceneManager() : currentScene(nullptr), currentWorld(nullptr) {}
	~SceneManager();

	static SceneManager*	instance;
	IScene*					currentScene;
	World*					currentWorld;
	vector<IScene*>			sceneList;
	vector<SCENE_MSG>		messageQueue;

	float	screenWidth;
	float	screenHeight;
};