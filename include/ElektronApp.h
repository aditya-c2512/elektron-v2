#pragma once
#include "Window.h"
#include "./drawables/Drawable.h"
#include "GuiManager.h"
#include "Camera.h"
#include "PointLight.h"
#include "drawables/ModelGraph.h"
#include "drawables/SkySphere.h"
#include "bindables/ElekTexMap.h"

class ElektronApp
{
public:
	ElektronApp();
	int Run();
private:
	void RunFrame();
	double GetCPUDiagnostics();
	void SpawnViewportWindow();
private:
	float width, height;

	GuiManager gui;
	Window wnd;

	float dt = 0.01f;
	float speed_factor = 1.0f;

	Camera cam;
	ElekTexMap elekTexMap;
	ModelGraph modelGraph;
	SkySphere skySphere;
	PointLight pointLight;

	MEMORYSTATUSEX memInfo;
	double ram_usage;

	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;
};

