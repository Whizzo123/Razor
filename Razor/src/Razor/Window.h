#pragma once

#include "Core.h"
#include "../Platform/Generic/IWindowProvider.h"

namespace Razor
{
    class   Window
    {
    public:
        Window(int width, int height, std::shared_ptr<IWindowProvider> Provider);
        ~Window();
        std::shared_ptr<IWindowProvider> GetWindowProvider();
        bool ShouldWindowClose() { return Provider->ShouldWindowClose(); }
        void SetWindowToClose() { Provider->SetWindowToClose(); }
        int GetWidth() const { return Width; }
        int GetHeight() const { return Height; }

    protected:
        std::shared_ptr<IWindowProvider> Provider;
        int Width;
        int Height;
    };
}

