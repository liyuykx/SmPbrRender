#include "WindowManager.h"

unsigned int WindowManager::SCR_WIDTH = 1200;
unsigned int WindowManager::SCR_HEIGHT = 900;
//use to move exmale view
Camera * WindowManager::camera = nullptr;
bool WindowManager::firstMouse = true;
bool WindowManager::mouseCapture = false;
double WindowManager::lastX = SCR_WIDTH / 2.0f;
double WindowManager::lastY = SCR_HEIGHT / 2.0f;


WindowManager::WindowManager(bool& success, unsigned int scrW, unsigned int scrH, std::string name) : name(name)
{

	WindowManager::SCR_WIDTH = scrW;
	WindowManager::SCR_HEIGHT = scrH;
	
	success = 1;
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);

	// glfw window creation										 
	w = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name.c_str(), NULL, NULL);
	if (!w)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		success = 0;
		return;
	}
	glfwMakeContextCurrent(w);
	glfwSetFramebufferSizeCallback(w, &WindowManager::framebuffer_size_callback);
	glfwSetCursorPosCallback(w, &WindowManager::mouse_callback);


	success = gladLoader() && success;
	if (success) {
		std::cout << "GLFW window correctly initialized!" << std::endl;
	}
}

int WindowManager::gladLoader() {
	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 0;
	}
	return 1;
}

// glfw: whenever the mouse moves, this callback is called
void WindowManager::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse == true) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	float deltaX, deltaY;
	deltaX = static_cast<float>(xpos - lastX);
	deltaY = static_cast<float>(ypos - lastY);
	lastX = xpos;
	lastY = ypos;
	if (WindowManager::mouseCapture) return;
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_PRESS)
	{
		camera->processMouseMovement(deltaX, deltaY);
	}
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void WindowManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	//glViewport(0, 0, width, height);
	WindowManager::SCR_WIDTH = width;
	WindowManager::SCR_HEIGHT = height;
}

void WindowManager::processInput()
{
	if (glfwGetKey(w, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(w, true);
	}
	else if (glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->setSpeedZ(0.01f);
	}
	else if (glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->setSpeedZ(-0.01f);
	}
	else {
		camera->setSpeedZ(0);
	}

	if (glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->setSpeedX(0.01f);
	}
	else if (glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->setSpeedX(-0.01f);
	}
	else {
		camera->setSpeedX(0);
	}

	if (glfwGetKey(w, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera->setSpeedY(-0.01f);
	}
	else if (glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera->setSpeedY(0.01f);
	}
	else {
		camera->setSpeedY(0);
	}

}

WindowManager::~WindowManager()
{
	terminate();
}
