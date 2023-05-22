#pragma once
#include "Window.h"
#include "./drawables/Drawable.h"
#include "GuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "drawables/ModelGraph.h"
#include "drawables/SkySphere.h"

class ElektronApp
{
public:
	ElektronApp();
	int Run();
private:
	void RunFrame();
	void SpawnModelControlWindow();
	void SpawnViewportWindow();
private:
	float width, height;

	GuiManager gui;
	Window wnd;

	float dt = 0.01f;
	float speed_factor = 1.0f;

	Camera cam;
	PointLight pointLight;

	ModelGraph modelGraph;
	SkySphere skySphere;
};

