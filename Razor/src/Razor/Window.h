#pragma once

#include "Core.h"
#include "../Platform/Generic/IWindowProvider.h"

namespace Razor
{
    class RAZOR_API Window
    {
    public:
        Window(int width, int height, Ref<IWindowProvider> Provider);
        ~Window() {};

        Ref<IWindowProvider> GetWindowProvider();
        bool ShouldWindowClose();
        void SetWindowToClose();
        int GetWidth() const;
        int GetHeight() const;
        /**
        * Setter function for window width
        * 
        * @param value - the value to set Window Width to
        */
        void SetWidth(int value);
        /**
        * Setter function for window height
        *
        * @param value - the value to set Window Height to
        */
        void SetHeight(int value);

    protected:
        Ref<IWindowProvider> _mProvider;
        int _mWidth;
        int _mHeight;
    };
}

