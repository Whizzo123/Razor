#include "../../Core.h"

namespace Razor
{
    struct Bitmap
    {
        unsigned int width;
        unsigned int rows;
        Scope<unsigned char[]> buffer;
    };
}