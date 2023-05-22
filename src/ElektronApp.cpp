#include "../include/ElektronApp.h"
#include <memory>

#include "../imgui/imgui.h"

ElektronApp::ElektronApp() : dt(0.01f), width(1920), height(1080), 
							wnd(1920,1080,L"Elektron Engine V2.0"), pointLight(wnd.GetGfx()),
							modelGraph(wnd.GetGfx(), "C:/Projects/elektron-v2/assets/models/helmet/helmet.obj"),
							skySphere(wnd.GetGfx())
{
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

	skySphere.Draw(wnd.GetGfx());
	modelGraph.Draw(wnd.GetGfx());
	pointLight.Draw(wnd.GetGfx());

	if (ImGui::Begin("Demo Simulation Speed", NULL, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::SliderFloat("Delta Time", &speed_factor, 0.0f, 100.0f);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	modelGraph.SpawnModelGraphControlWindow();
	//SpawnViewportWindow(); // TO-DO: Separate viewport window for rendering.
	cam.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, height / width, cam.near_plane, cam.far_plane));

	wnd.GetGfx().PresentFrame();
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
	//ImGui::Image((void*)wnd.GetGfx().GetRTV().Get(), ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::End();

}
