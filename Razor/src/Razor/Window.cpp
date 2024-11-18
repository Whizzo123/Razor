#include "Window.h"

namespace Razor
{
    Window::Window(int width, int height, std::shared_ptr<IWindowProvider> Provider)
    {
        Provider->CreateProviderWindow(width, height, "Edge");
        this->Provider = Provider;
        Width = width;
        Height = height;
    }

    Window::~Window()
    {

    }


    std::shared_ptr<IWindowProvider> Window::GetWindowProvider()
    {
        return Provider;
    }
}
