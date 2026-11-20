#pragma once

#include "../../Utils/Vector2u.h"
#include "../../Utils/Vector2i.h"

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