#include "Vector4.h"

namespace Razor {
    Vector4 Vector4::operator/(const float rhs)
    {
        Vector4 ret = {this->X, this->Y, this->Z, this->W};
        ret.X /= rhs;
        ret.Y /= rhs;
        ret.Z /= rhs;
        ret.W /= rhs;
        return ret;
    }
    void Vector4::operator/=(const float rhs)
    {
        this->X /= rhs;
        this->Y /= rhs;
        this->Z /= rhs;
        this->W /= rhs;
    }
}