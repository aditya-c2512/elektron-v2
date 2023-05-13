#pragma once
#include "Window.h"
#include "./drawables/Drawable.h"
#include "GuiManager.h"
#include "Camera.h"
#include "PointLight.h"

class ElektronApp
{
public:
	ElektronApp();
	int Run();
private:
	void RunFrame();
private:
	float width, height;

	GuiManager gui;

	Window wnd;

	float dt = 0.01f;
	float speed_factor = 1.0f;

	Camera cam;
	PointLight pointLight;

	std::vector<std::unique_ptr<Drawable>> drawables;
	static constexpr size_t nDrawables = 1;
};

