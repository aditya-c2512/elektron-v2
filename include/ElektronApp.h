#pragma once
#include "Window.h"
#include "Drawable.h"

class ElektronApp
{
public:
	ElektronApp();
	int Run();
private:
	void RunFrame();
private:
	Window wnd;
	std::vector<std::unique_ptr<Drawable>> drawables;
	static constexpr size_t nDrawables = 80;
};

