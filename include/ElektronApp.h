#pragma once
#include "Window.h"

class ElektronApp
{
public:
	ElektronApp();
	int Run();
private:
	void RunFrame();
private:
	Window wnd;
};

