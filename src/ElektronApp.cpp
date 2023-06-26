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

	wnd.GetGfx().SetCamera(cam.GetMatrix());

	pointLight.Bind(wnd.GetGfx(), cam.GetMatrix());
	
	skySphere.Draw(wnd.GetGfx());
	modelGraph.Draw(wnd.GetGfx());
	pointLight.Draw(wnd.GetGfx());

	/*while (const auto e = wnd.keyboard.ReadKey())
	{
		if (!e->isPressed())
		{
			continue;
		}

		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		}
	}*/

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
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.keyboard.KeyIsPressed('A'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.keyboard.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.keyboard.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.keyboard.KeyIsPressed('R'))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.keyboard.KeyIsPressed('F'))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
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
	cam.SpawnControlWindow();
	pointLight.SpawnControlWindow();

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveFovLH((cam.fov * PI)/180.0f, width / height, cam.near_plane, cam.far_plane));

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
