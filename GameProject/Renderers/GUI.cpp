#include "stdafx.h"
#include "Gui.h"

Gui::Gui(GlobalValues* values)
	: Renderers(values)
{

}

Gui::~Gui()
{

}

void Gui::Update()
{
	if (Keyboard::Get()->Press(VK_CONTROL))
	{
		if (Keyboard::Get()->Down(VK_F2))
			ChangeShowEnvironmentWindow();

		if (Keyboard::Get()->Down(VK_F11))
			ChangeShowSystemInfoWindow();

		if (Keyboard::Get()->Down(VK_F12))
			ChangeShowDemoWindow();
	}
}

void Gui::PreRender()
{

}

void Gui::Render()
{

}

void Gui::PostRender()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Environment", "Ctrl+F2"))
				ChangeShowEnvironmentWindow();

			if (ImGui::MenuItem("System", "Ctrl+F11"))
				ChangeShowSystemInfoWindow();

			if (ImGui::MenuItem("Demo", "Ctrl+F12"))
				ChangeShowDemoWindow();

			ImGui::EndMenu();
		}//if(BeiginMenu)

		ImGui::EndMainMenuBar();
	}

	if (values->GuiSettings->bShowSystemInfoWindow == true)
	{
		ImGui::Begin("System Info", &values->GuiSettings->bShowSystemInfoWindow);
		D3DXVECTOR3 position;
		values->MainCamera->GetPosition(&position);
		ImGui::Text("Camera Position : %.0f, %.0f, %.0f", position.x, position.y, position.z);

		D3DXVECTOR2 rotation;
		values->MainCamera->GetRotationDegree(&rotation);
		ImGui::Text("Camera Rotation : %.0f, %.0f", rotation.x, rotation.y);

		ImGui::Separator();

		ImGui::Text("Frame Per Second : %4d", (int)ImGui::GetIO().Framerate);
		ImGui::End();
	}

	if (values->GuiSettings->bShowDemoWindow == true)
	{
		ImGui::ShowDemoWindow(&values->GuiSettings->bShowDemoWindow);
	}
}

void Gui::ResizeScreen()
{

}

void Gui::ChangeShowDemoWindow()
{
	values->GuiSettings->bShowDemoWindow = !values->GuiSettings->bShowDemoWindow;
}

void Gui::ChangeShowEnvironmentWindow()
{
	values->GuiSettings->bShowEnvironmentWindow = !values->GuiSettings->bShowEnvironmentWindow;
}

void Gui::ChangeShowSystemInfoWindow()
{
	values->GuiSettings->bShowSystemInfoWindow = !values->GuiSettings->bShowSystemInfoWindow;
}
