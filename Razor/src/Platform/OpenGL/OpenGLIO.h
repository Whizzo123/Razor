#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../Generic/IPlatformIO.h"

class OpenGLIO : public IPlatformIO
{
public:
	OpenGLIO(GLFWwindow* Window);
	~OpenGLIO() {};
	void RegisterInputCallbacks() override;
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

private:
	GLFWwindow* Window;
};

