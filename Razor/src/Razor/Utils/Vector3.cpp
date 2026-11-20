#include "Vector3.h"
#include <glm/glm.hpp>

namespace Razor {
    
    Vector3 Vector3::operator+(const Vector3& vec)
    {
        Vector3 ret{this->X, this->Y, this->Z};
        ret.X += vec.X;
        ret.Y += vec.Y;
        ret.Z += vec.Z;
        return ret;
    }

    void Vector3::operator+=(const Vector3& vec)
    {
        this->X += vec.X;
        this->Y += vec.Y;
        this->Z += vec.Z;
    }

    Vector3 Vector3::operator-(const Vector3& vec)
    {
        Vector3 ret{this->X, this->Y, this->Z};
        ret.X -= vec.X;
        ret.Y -= vec.Y;
        ret.Z -= vec.Z;
        return ret;
    }

    void Vector3::operator-=(const Vector3& vec)
    {
        this->X -= vec.X;
        this->Y -= vec.Y;
        this->Z -= vec.Z;
    }

    Vector3 Vector3::operator*(const float rhs) const
    {
        Vector3 vec = Vector3{this->X, this->Y, this->Z};
        vec *= rhs;
        return vec;
    }

    void Vector3::operator*=(const float rhs) 
    {
        this->X *= rhs;
        this->Y *= rhs;
        this->Z *= rhs;
    }

    Vector3 Vector3::Normalise(Vector3 vec)
    {
        glm::vec3 norm = glm::normalize(glm::vec3(vec.X, vec.Y, vec.Z));
        Vector3 ret {norm.x, norm.y, norm.z};
        return ret;
    }

    Vector3 Vector3::Cross(Vector3 a, Vector3 b)
    {
        glm::vec3 cross = glm::cross(glm::vec3(a.X, a.Y, a.Z), glm::vec3(b.X, b.Y, b.Z));
        Vector3 ret {cross.x, cross.y, cross.z};
        return ret;
    }

}