#pragma once
#include "../Generic/IWindowProvider.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"

namespace Razor
{
	class OpenGLWindowProvider : public IWindowProvider
	{
	public:
		OpenGLWindowProvider() : Window(nullptr), Width(0), Height(0) {};
		~OpenGLWindowProvider() {};
		void CreateProviderWindow(int Width, int Height, const std::string& Title) override;
		bool ShouldWindowClose() override;
		void SetWindowToClose() override;
		GLFWwindow* GetPlatformWindowPtr() { return Window; };
	private:
		GLFWwindow* Window;
		int Width;
		int Height;
	};
}

