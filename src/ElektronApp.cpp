#include "../include/ElektronApp.h"
#include <memory>

#include "../imgui/imgui.h"

ElektronApp::ElektronApp() : dt(0.01f), width(1920), height(1080), 
							wnd(1920,1080,L"Elektron Engine V2.0"), pointLight(wnd.GetGfx()),
							modelGraph(wnd.GetGfx(), "C:/Projects/elektron-v2/assets/models/nanosuit/nanosuit.obj")
{
	/*drawables.reserve(nDrawables);

	for (auto i = 0; i < nDrawables; i++)
	{
		drawables.push_back(std::make_unique<Model>(
			wnd.GetGfx(), "C:/Projects/elektron-v2/assets/models/bunny.obj"
			)
		);
	}*/

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, height/width, 0.5f, 50.0f));
}

int ElektronApp::Run()
{
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}
		RunFrame();
	}
}

void ElektronApp::RunFrame()
{
	dt = ImGui::GetIO().DeltaTime * speed_factor;

	wnd.GetGfx().BeginFrame(0.129f, 0.148f, 0.179f);

	wnd.GetGfx().SetCamera(cam.GetMatrix());

	pointLight.Bind(wnd.GetGfx(), cam.GetMatrix());

	/*for (auto& drawable : drawables)
	{
		drawable->Update(dt);
		drawable->Draw(wnd.GetGfx());
		drawable->SpawnControlWindow();
	}*/

	const auto transform = dx::XMMatrixRotationRollPitchYaw(pos.pitch, pos.yaw, pos.roll) * dx::XMMatrixTranslation(pos.x, pos.y, pos.z);
	modelGraph.Draw(wnd.GetGfx(), transform);

	pointLight.Draw(wnd.GetGfx());

	if (ImGui::Begin("Demo Simulation Speed", NULL, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::SliderFloat("Delta Time", &speed_factor, 0.0f, 100.0f);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	/*if (ImGui::Begin("Test"))
	{
		if (ImGui::Button("Import asset"))
		{
			ImGui::OpenPopup("ImportAssetDialog");
			
		}
		if (ImGui::BeginPopupModal("ImportAssetDialog"))
		{
			ImGui::Text("test");
			ImGui::EndPopup();
		}
	}
	ImGui::End();*/

	SpawnModelControlWindow();
	//SpawnViewportWindow(); // TO-DO: Separate viewport window for rendering.
	cam.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, height / width, cam.near_plane, cam.far_plane));

	wnd.GetGfx().PresentFrame();
}

void ElektronApp::SpawnModelControlWindow()
{
	if (ImGui::Begin("Model", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("POSITION");
		ImGui::DragFloat3("World Space", &pos.x, 0.001f, -1000.0f, 1000.0f, "%.1f");
		ImGui::Text("ROTATION");
		ImGui::SliderAngle("Roll", &pos.roll, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Pitch", &pos.pitch, -180.0f, 180.0f, "%.1f");
		ImGui::SliderAngle("Yaw", &pos.yaw, -180.0f, 180.0f, "%.1f");
		if (ImGui::Button("RESET"))
		{
			pos.x = pos.y = pos.z = 0.0f;
			pos.roll = pos.pitch = pos.yaw = 0.0f;
		}
	}
	ImGui::End();
}

void ElektronApp::SpawnViewportWindow()
{
	ImGui::Begin("Viewport");

	// Get the position and size of the viewport window
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImVec2 size = ImGui::GetContentRegionAvail();

	// Draw a frame around the viewport
	ImGui::GetWindowDrawList()->AddRect(pos, ImVec2(pos.x + size.x, pos.y + size.y), IM_COL32(255, 255, 255, 255));

	// Render your frames within the viewport

	ImGui::End();

}
