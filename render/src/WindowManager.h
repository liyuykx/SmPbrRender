#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include <iostream>
class WindowManager
{
public:
	//initializer
	WindowManager(bool& success, unsigned int SCR_WIDTH = 800, unsigned int SCR_HEIGHT = 600, std::string name = "render");
	~WindowManager();
	GLFWwindow * w;
	GLFWwindow * getWindow() const { return w; }
	// screen settings
	static unsigned int SCR_WIDTH;
	static unsigned int SCR_HEIGHT;

	void terminate() {
		glfwTerminate();
	}

	// return if the main loop must continue
	bool continueLoop() {
		return !glfwWindowShouldClose(this->w);
	}

	//put this at the end of the main
	void swapBuffersAndPollEvents() {
		glfwSwapBuffers(this->w);
		glfwPollEvents();
	}

	void processInput();//input handler

	static Camera * camera;
	static bool mouseCapture;
private:
	int gladLoader(); // set mouse input and load opengl functions 

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	static void mouse_callback(GLFWwindow* window,double xpos,double ypos);
	

	//avoid to make the mouse to jump at the start of the program
	static bool firstMouse;// = true;
	static double lastX;
	static double lastY;

	std::string name;
};


