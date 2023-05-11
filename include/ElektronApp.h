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
	float dt;
	float speed_factor = 1.0f;

	Camera cam;
	

	Window wnd;

	PointLight pointLight;
	std::vector<std::unique_ptr<Drawable>> drawables;
	static constexpr size_t nDrawables = 1;
};

