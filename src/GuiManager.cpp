#include "../include/GuiManager.h"
#include "../imgui/imgui.h"

GuiManager::GuiManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

GuiManager::~GuiManager()
{
	ImGui::DestroyContext();
}
