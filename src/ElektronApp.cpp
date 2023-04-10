#include "../include/ElektronApp.h"

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
	wnd.GetGfx().ClearBuffer(0.5f, 0.3f, 0.1f);
	wnd.GetGfx().PresentFrame();
}
