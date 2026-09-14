#include "OpenGLWindowProvider.h"
#include <iostream>
#include "../../Razor/Engine.h"
#include "../../Razor/Window.h"
#include "../../Razor/Log.h"


namespace Razor
{
    void framebuffer_size_callback([[maybe_unused]]GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        Engine::Get().GetWindow().SetWidth(width);
        Engine::Get().GetWindow().SetHeight(height);
        // Think what we want is to re-create our framebuffers to be of this width and height rather than try resizing them
        ///Renderer->ResizeFramebuffers(width, height);
    }

	void OpenGLWindowProvider::CreateProviderWindow(int Width, int Height, const std::string& Title)
	{
        std::cout << "Creating window" << std::endl;
        Window = glfwCreateWindow(Width, Height, Title.c_str(), nullptr, nullptr);

        if (Window == nullptr) {
            const char* buffer_ptr = new char[512];
            glfwGetError(&buffer_ptr);
            std::string buffer_str = buffer_ptr;
            RZ_CORE_ERROR("Failed to create window GLFW {0}", buffer_str);
            glfwTerminate();
            return;
        } else {
            RZ_CORE_INFO("OpenGLWindowProvider::CreateProviderWindow -> Successfully Created Window");
        }

        glfwMakeContextCurrent(Window);

        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            std::cout << "Failed to initialise GLAD" << std::endl;
        }

        const GLubyte *vendor = glGetString(GL_VENDOR);
        const GLubyte *renderer = glGetString(GL_RENDERER);
        const GLubyte *version = glGetString(GL_VERSION);
        RZ_CORE_INFO("OpenGL Vendor: {0}", reinterpret_cast<const char*>(vendor));
        RZ_CORE_INFO("OpenGL Renderer: {0}", reinterpret_cast<const char*>(renderer));
        RZ_CORE_INFO("OpenGL Version: {0}", reinterpret_cast<const char*>(version));

        glfwSwapInterval(0);

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