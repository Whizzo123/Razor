#pragma once

#include "Core.h"
#include "../Platform/Generic/IWindowProvider.h"

namespace Razor
{
    class RAZOR_API Window
    {
    public:
        Window(int width, int height, std::shared_ptr<IWindowProvider> Provider);
        ~Window() {};

        std::shared_ptr<IWindowProvider> GetWindowProvider();
        bool ShouldWindowClose();
        void SetWindowToClose();
        int GetWidth() const;
        int GetHeight() const;
        /**
        * Setter function for window width
        * 
        * @param Value - the value to set Window Width to
        */
        void SetWidth(int Value);
        /**
        * Setter function for window height
        *
        * @param Value - the value to set Window Height to
        */
        void SetHeight(int Value);

    protected:
        std::shared_ptr<IWindowProvider> Provider;
        int Width;
        int Height;
    };
}

