#include "../include/ElektronApp.h"

float angle = 0.0f;

ElektronApp::ElektronApp() : wnd(800,800,L"Elektron Engine V2.0")
{
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
	wnd.GetGfx().ClearBuffer(0.0f, 0.0f, 1.0f);
	wnd.GetGfx().DrawTestTriangle(angle);
	wnd.GetGfx().PresentFrame();
	angle += 0.1f;
}
