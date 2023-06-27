#include "../include/ElektronApp.h"
#include <memory>
#include "../imgui/imgui.h"

#define PI 3.14159f

ElektronApp::ElektronApp() : dt(0.01f), width(1920), height(1080), 
							wnd(1920,1080,L"Elektron Engine V2.0"),
							modelGraph(wnd.GetGfx(), elekTexMap, "C:/Projects/elektron-v2/assets/models/sponza/", "Sponza.gltf", ModelGraph::ELEKTRON_MODEL_FORMAT::ELEKTRON_MODEL_GLTF),
							skySphere(wnd.GetGfx(), elekTexMap),
							pointLight(wnd.GetGfx())
{
	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, height/width, 0.5f, 500.0f));

	cameras.AddCamera(std::make_unique<Camera>(wnd.GetGfx(), "Free Look #1", dx::XMFLOAT3{ -13.5f,6.0f,3.5f }, 0.0f, PI / 2.0f));
	cameras.AddCamera(std::make_unique<Camera>(wnd.GetGfx(), "Free Look #2", dx::XMFLOAT3{ -13.5f,28.8f,-6.4f }, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));

	memInfo.dwLength = sizeof(MEMORYSTATUSEX);
	GlobalMemoryStatusEx(&memInfo);

	SYSTEM_INFO sysInfo;
	FILETIME ftime, fsys, fuser;

	GetSystemInfo(&sysInfo);
	numProcessors = sysInfo.dwNumberOfProcessors;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&lastCPU, &ftime, sizeof(FILETIME));

	self = GetCurrentProcess();
	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
	memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double ElektronApp::GetCPUDiagnostics() 
{
	FILETIME ftime, fsys, fuser;
	ULARGE_INTEGER now, sys, user;
	double percent;

	GetSystemTimeAsFileTime(&ftime);
	memcpy(&now, &ftime, sizeof(FILETIME));

	GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
	memcpy(&sys, &fsys, sizeof(FILETIME));
	memcpy(&user, &fuser, sizeof(FILETIME));
	percent = (sys.QuadPart - lastSysCPU.QuadPart) +
		(user.QuadPart - lastUserCPU.QuadPart);
	percent /= (now.QuadPart - lastCPU.QuadPart);
	percent /= numProcessors;
	lastCPU = now;
	lastUserCPU = user;
	lastSysCPU = sys;

	return percent;
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
	ram_usage = (memInfo.ullTotalPhys - memInfo.ullAvailPhys)/pow(10,9);

	wnd.GetGfx().BeginFrame(0.129f, 0.148f, 0.179f);

	cameras.Bind(wnd.GetGfx());
	pointLight.Bind(wnd.GetGfx(), cameras.GetCamera().GetMatrix());
	
	skySphere.Draw(wnd.GetGfx());
	modelGraph.Draw(wnd.GetGfx());
	pointLight.Draw(wnd.GetGfx());
	cameras.Draw(wnd.GetGfx());

	if (wnd.mouse.RightIsPressed())
	{
		wnd.DisableCursor();
		wnd.mouse.EnableRaw();
	}
	else
	{
		wnd.EnableCursor();
		wnd.mouse.DisableRaw();
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.keyboard.KeyIsPressed('W'))
		{
			cameras.GetCamera().Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.keyboard.KeyIsPressed('A'))
		{
			cameras.GetCamera().Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.keyboard.KeyIsPressed('S'))
		{
			cameras.GetCamera().Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.keyboard.KeyIsPressed('D'))
		{
			cameras.GetCamera().Translate({ dt,0.0f,0.0f });
		}
		if (wnd.keyboard.KeyIsPressed('R'))
		{
			cameras.GetCamera().Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.keyboard.KeyIsPressed('F'))
		{
			cameras.GetCamera().Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cameras.GetCamera().Rotate((float)delta->x, (float)delta->y);
		}
	}

	if (ImGui::Begin("Engine Diagnostics", NULL, ImGuiWindowFlags_NoCollapse|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoResize|ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Application Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("CPU Usage : %.1f %%", GetCPUDiagnostics());
		ImGui::Text("RAM Usage : %.2f GB", ram_usage);
	}
	ImGui::End();

	modelGraph.SpawnModelGraphControlWindow();
	cameras.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveFovLH((cameras.GetCamera().fov * PI)/180.0f, width / height, cameras.GetCamera().near_plane, cameras.GetCamera().far_plane));

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
