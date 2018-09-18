#pragma once

class Program
{
public:
	Program();
	~Program();

	void Update();
	void LateUpdate();

	void PreRender();
	void Render();
	void PostRender();

	void ResizeScreen();

private:
	struct ExecuteValues* values;
	vector<class Execute *> executes;
};