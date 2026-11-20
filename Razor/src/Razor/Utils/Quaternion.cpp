#include "Quaternion.h"
#include <glm/glm.hpp>

namespace Razor {
    Quaternion::Quaternion(float w, float x, float y, float z) {
        _mW = w;
        _mX = x;
        _mY = y;
        _mZ = z;
    }

    Quaternion::Quaternion(Vector3 vec) {
        _mW = 1.0f;
        _mX = vec.X;
        _mY = vec.Y;
        _mZ = vec.Z;
    }

    float Quaternion::W(){
        return _mW;
    }
    float Quaternion::X(){
        return _mX;
    }
    float Quaternion::Y(){
        return _mY;
    }
    float Quaternion::Z(){
        return _mZ;
    }
}