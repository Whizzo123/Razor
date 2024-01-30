#include "OpenGLIO.h"

std::unordered_map<int, RazorKey> OpenGLIO::GLFWToCustomKeyMap = {
		{GLFW_KEY_W, RazorKey::W},
		{GLFW_KEY_A, RazorKey::A},
		{GLFW_KEY_S, RazorKey::S},
		{GLFW_KEY_D, RazorKey::D},
		{GLFW_KEY_ESCAPE, RazorKey::Escape}
};
std::unordered_map<int, RazorKeyState> OpenGLIO::GLFWToCustomStateMap = {
		{GLFW_PRESS, KEY_PRESSED},
		{GLFW_RELEASE, KEY_RELEASED}
};

OpenGLIO::OpenGLIO(GLFWwindow* Window) : Window(Window)
{
	
}

void OpenGLIO::RegisterInputCallbacks()
{
	glfwSetMouseButtonCallback(Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			MouseButtonCallback(window, button, action, mods);
		});
	glfwSetKeyCallback(Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
		{
			KeyCallback(window, key, action);
		});
	glfwSetCursorPosCallback(Window, [](GLFWwindow* window, double Xpos, double Ypos)
		{
			CursorPosCallback(window, Xpos, Ypos);
		});
}

void OpenGLIO::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	RazorIO& EngineIO = RazorIO::Get();
	if (action == GLFW_PRESS)
	{
		EngineIO.OnMouseButtonPressed().ExecuteIfBound(button);
	}
	else
	{
		EngineIO.OnMouseButtonReleased().ExecuteIfBound(button);
	}
}

void OpenGLIO::KeyCallback(GLFWwindow* window, int button, int action)
{
	RazorIO& EngineIO = RazorIO::Get();
	EngineIO.OnKeyStateChanged().ExecuteIfBound(GLFWToCustomKeyMap[button], GLFWToCustomStateMap[action]);
}

void OpenGLIO::CursorPosCallback(GLFWwindow* window, double Xpos, double Ypos)
{
	RazorIO& EngineIO = RazorIO::Get();
	EngineIO.OnMousePosChanged().ExecuteIfBound(Xpos, Ypos);
}