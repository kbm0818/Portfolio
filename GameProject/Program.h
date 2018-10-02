#pragma once

class Program
{
public:
	Program();
	~Program();

	void Init();

	void Update();
	void PreRender();
	void Render();
	void PostRender();
	void ResizeScreen();

private:
	void SetGlobalValues();

private:
	struct GlobalValues* values;
	vector<class Renderers *> renderer;
};