#include "../include/ElektronApp.h"
#include "../include/Box.h"
#include <memory>

float angle = 0.01f;

ElektronApp::ElektronApp() : wnd(1000,1000,L"Elektron Engine V2.0")
{
	std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	drawables.reserve(nDrawables);

	for (auto i = 0; i < nDrawables; i++)
	{
		drawables.push_back(std::make_unique<Box>(
			wnd.GetGfx(), rng, adist,
			ddist, odist, rdist
			)
		);
	}

	wnd.GetGfx().SetProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 1.0f, 0.5f, 40.0f));
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
	wnd.GetGfx().ClearBuffer(1.0f, 0.0f, 0.0f);
	for (auto& drawable : drawables)
	{
		drawable->Update(angle);
		drawable->Draw(wnd.GetGfx());
	}
	wnd.GetGfx().PresentFrame();
}
