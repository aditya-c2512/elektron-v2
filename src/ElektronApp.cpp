#include "../include/ElektronApp.h"
#include "../include/Model.h"
#include <memory>

float angle = 0.01f;

ElektronApp::ElektronApp() : wnd(1000,1000,L"Elektron Engine V2.0")
{
	drawables.reserve(nDrawables);

	for (auto i = 0; i < nDrawables; i++)
	{
		drawables.push_back(std::make_unique<Model>(
			wnd.GetGfx(), "C:/Projects/elektron-v2/assets/models/bunny.obj"
			)
		);
	}

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 5.0f));
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
	wnd.GetGfx().ClearBuffer(0.129f, 0.148f, 0.179f);
	for (auto& drawable : drawables)
	{
		drawable->Update(angle);
		drawable->Draw(wnd.GetGfx());
	}
	wnd.GetGfx().PresentFrame();
}
