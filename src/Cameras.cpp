#include "../include/Cameras.h"
#include "../imgui/imgui.h"
#include "../include/Camera.h"
#include "../include/ElektronGFX.h"

void Cameras::SpawnControlWindow()
{
	if (ImGui::Begin("Cameras", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::BeginCombo("Active Camera", cameras[selected]->GetName().c_str()))
		{
			for (int i = 0; i < cameras.size(); i++)
			{
				const bool isSelectable = (i == selected);
				if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelectable))
				{
					selected = i;
				}
			}
			ImGui::EndCombo();
		}

		GetCamera().SpawnControlWindow();
	}
	ImGui::End();
}

void Cameras::Bind(ElektronGFX& gfx)
{
	gfx.SetCamera(GetCamera().GetMatrix());
}

void Cameras::Draw(ElektronGFX& gfx)
{
	for (int i = 0; i < cameras.size(); i++)
	{
		if(i != selected) cameras[i]->Draw(gfx);
	}
}

void Cameras::AddCamera(std::unique_ptr<Camera> pCam)
{
	cameras.push_back(std::move(pCam));
}

Camera& Cameras::GetCamera()
{
	return *cameras[selected];
}

Cameras::~Cameras()
{
}
