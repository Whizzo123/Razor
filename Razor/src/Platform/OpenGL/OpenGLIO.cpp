#include "OpenGLIO.h"
#include "../../Razor/IO/RazorIO.h"


OpenGLIO::OpenGLIO(GLFWwindow* Window) : Window(Window)
{
	
}

void OpenGLIO::RegisterInputCallbacks()
{
	glfwSetMouseButtonCallback(Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			MouseButtonCallback(window, button, action, mods);
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
