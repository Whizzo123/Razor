#pragma once
#include "Vector3.h"

namespace Razor {
    class Quaternion {
    public:
        Quaternion(float w, float x, float y, float z);
        Quaternion(Vector3 vec);

        float W();
        float X();
        float Y();
        float Z();

    private:
        float _mW;
        float _mX;
        float _mY;
        float _mZ;
    };
}