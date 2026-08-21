#pragma once
#include <string>
#include <iostream>

namespace Razor
{
    class TextureReader
    {
    public:
        static unsigned int CreateTexture(const std::string& fileName);

    };
}
