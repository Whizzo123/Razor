#pragma once

#include "../../Utils/Vector.h"

namespace Razor
{
    struct Character
    {
        unsigned int mTextureID;
        Vector2u mSize;
        Vector2i mBearing;
        unsigned int mAdvance;
    };
}