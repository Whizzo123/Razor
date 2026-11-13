#include "Vector2.h"
#include <glm/glm.hpp>

namespace Razor{

    Vector2 Vector2::operator-(const Vector2& vec)
    {
        Vector2 ret{this->X, this->Y};
        ret.X -= vec.X;
        ret.Y -= vec.Y;
        return ret;
    }

    Vector2 Vector2::Normalise(Vector2 vec)
    {
        glm::vec2 norm = glm::normalise(glm::vec2(vec.X, vec.Y));
        Vector2 ret {norm.x, norm.y};
        return ret;
    }

    Vector2 Vector2::Cross(Vector2 a, Vector2 b)
    {
        glm::vec2 cross = glm::cross(glm::vec2(a.X, a.Y), glm::vec2(b.x, b.y));
        Vector2 ret {cross.x, cross.y};
        return ret;
    }

}