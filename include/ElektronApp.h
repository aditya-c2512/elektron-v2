#pragma once
#include "Window.h"
#include "./drawables/Drawable.h"
#include "GuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "drawables/ModelGraph.h"

class ElektronApp
{
public:
	ElektronApp();
	int Run();
private:
	void RunFrame();
	void SpawnModelControlWindow();
private:
	float width, height;

	GuiManager gui;
	Window wnd;

	float dt = 0.01f;
	float speed_factor = 1.0f;

	Camera cam;
	PointLight pointLight;

	ModelGraph modelGraph;

	struct
	{
		float roll = -90.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	} pos;

	/*std::vector<std::unique_ptr<Drawable>> drawables;
	static constexpr size_t nDrawables = 1;*/
};

