#pragma once

#include "Quaternion.h"
#include "Vector3.h"
#include "Vector4.h"

namespace Razor {
    class Matrix4 {
    public:
        Matrix4();
        Matrix4(float def);
        float* GetValuePtr();
        static Matrix4 Translate(Matrix4 matrix, Vector3 vec);
        static Matrix4 Scale(Matrix4 matrix, Vector3 vec);
        static Matrix4 ToMat4(Quaternion quat);
        static Matrix4 Perspective(float fovy, float aspect, float near, float far);
        static Matrix4 Ortho(float left, float right, float bottom, float top);
        static Matrix4 LookAt(Vector3 eye, Vector3 center, Vector3 up);
        static Matrix4 Inverse(Matrix4 mat);
        Matrix4 operator*(const Matrix4& rhs);
        Matrix4 operator*(const Vector4& rhs);
    private:
        float _mElements[16];
    };
}