#include "../include/ElektronApp.h"
#include "../include/Model.h"
#include <memory>

#include "../imgui/imgui.h"

ElektronApp::ElektronApp() : width(1920), height(1080), wnd(1920,1080,L"Elektron Engine V2.0")
{
	drawables.reserve(nDrawables);

	for (auto i = 0; i < nDrawables; i++)
	{
		drawables.push_back(std::make_unique<Model>(
			wnd.GetGfx(), "C:/Projects/elektron-v2/assets/models/bunny.obj"
			)
		);
	}

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, height/width, 0.5f, 5.0f));
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

	for (auto& drawable : drawables)
	{
		drawable->Update(dt);
		drawable->Draw(wnd.GetGfx());
	}

	static char buffer[1024];

	if (ImGui::Begin("Demo Simulation Speed"))
	{
		ImGui::SliderFloat("Delta Time", &speed_factor, 0.0f, 10.0f);
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
	ImGui::End();

	cam.SpawnControlWindow();

	wnd.GetGfx().PresentFrame();
}
