#include "GLFWTimeProvider.h"

#include "GLFW/glfw3.h"

namespace Razor
{
	float GLFWTimeProvider::GetTime()
	{
		return glfwGetTime();
	}
}
