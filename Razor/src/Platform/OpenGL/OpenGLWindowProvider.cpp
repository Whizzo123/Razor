#include "OpenGLWindowProvider.h"
#include <iostream>


namespace Razor
{
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        // Think what we want is to re-create our framebuffers to be of this width and height rather than try resizing them
        ///Renderer->ResizeFramebuffers(width, height);
    }

	void OpenGLWindowProvider::CreateProviderWindow(int Width, int Height, const std::string& Title)
	{
        std::cout << "Creating window" << std::endl;
        Window = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);

        if (Window == nullptr)
        {
            std::cout << "Failed to create window" << std::endl;
            glfwTerminate();
        }

        glfwMakeContextCurrent(Window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            std::cout << "Failed to initialise GLAD" << std::endl;
        }
        this->Width = Width;
        this->Height = Height;
        glViewport(0, 0, Width, Height);
        glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
	}

    bool OpenGLWindowProvider::ShouldWindowClose()
    {
        return glfwWindowShouldClose(Window);
    }

    void OpenGLWindowProvider::SetWindowToClose()
    {
        glfwSetWindowShouldClose(Window, true);
    }
}