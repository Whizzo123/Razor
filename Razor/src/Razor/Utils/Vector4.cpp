#include "Vector4.h"

namespace Razor {
    Vector4 operator/(const float rhs)
    {
        Vector4 ret = {this->X, this->Y, this->Z, this->W};
        ret.X /= rhs;
        ret.Y /= rhs;
        ret.Z /= rhs;
        ret.W /= rhs;
        return ret;
    }
    void operator/=(const float rhs)
    {
        this->X /= vec.X;
        this->Y /= vec.Y;
        this->Z /= vec.Z;
        this->W /= vec.W;
    }
}