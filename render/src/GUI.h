#pragma once
#include "WindowManager.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
class GUI 
{
public:
	GUI(WindowManager& w);
	~GUI();
	void draw();
	void update();
	void render();
	int getExampleID();
private:
	int ExampleID;
};

