#include "Window.h"

namespace Razor
{
    Window::Window(int width, int height, Ref<IWindowProvider> provider)
    {
        provider->CreateProviderWindow(width, height, "Edge");
        this->_mProvider = provider;
        _mWidth = width;
        _mHeight = height;
    }

    Ref<IWindowProvider> Window::GetWindowProvider()
    {
        return _mProvider;
    }

    bool Window::ShouldWindowClose()
    {
        return _mProvider->ShouldWindowClose();
    }

    void Window::SetWindowToClose()
    {
        _mProvider->SetWindowToClose();
    }

    int Window::GetWidth() const
    {
        return _mWidth;
    }

    int Window::GetHeight() const
    {
        return _mHeight;
    }

    void Window::SetWidth(int value)
    {
        _mWidth = value;
    }

    void Window::SetHeight(int value)
    {
        _mHeight = value;
    }
}
