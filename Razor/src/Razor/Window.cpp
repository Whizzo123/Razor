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

    std::shared_ptr<IWindowProvider> Window::GetWindowProvider()
    {
        return Provider;
    }

    bool Window::ShouldWindowClose()
    {
        return Provider->ShouldWindowClose();
    }

    void Window::SetWindowToClose()
    {
        Provider->SetWindowToClose();
    }

    int Window::GetWidth() const
    {
        return Width;
    }

    int Window::GetHeight() const
    {
        return Height;
    }
}
