#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../Razor/IO/RazorIO.h"
#include "../Generic/IPlatformIO.h"
#include <unordered_map>

class OpenGLIO : public IPlatformIO
{
public:
	OpenGLIO(GLFWwindow* Window);
	~OpenGLIO() {};
	void RegisterInputCallbacks() override;
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void KeyCallback(GLFWwindow* window, int button, int action);
	static void CursorPosCallback(GLFWwindow* window, double Xpos, double YPos);

private:
	GLFWwindow* Window;
	static std::unordered_map<int, RazorKey> GLFWToCustomKeyMap;
	static std::unordered_map<int, RazorKeyState> GLFWToCustomStateMap;
};

