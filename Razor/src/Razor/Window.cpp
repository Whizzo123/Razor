#include "Window.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


Window::Window(int width, int height)
{
    std::cout << "Creating window" << std::endl;
    window = glfwCreateWindow(800, 600, "Smart Renderer", nullptr, nullptr);

    if(window == nullptr)
    {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialise GLAD" << std::endl;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

Window::~Window()
{
    
}


GLFWwindow* Window::GetWindowPtr() const
{
    return window;
}
