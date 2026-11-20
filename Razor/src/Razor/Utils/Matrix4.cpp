#include "Matrix4.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Razor {

    Matrix4::Matrix4() {
        glm::mat4 mat = glm::mat4(1.0f);
        
        std::copy(glm::value_ptr(mat),
                  glm::value_ptr(mat) + 16,
                  _mElements);
    }
    Matrix4::Matrix4(float def) {
        glm::mat4 mat = glm::mat4(def);
        
        std::copy(glm::value_ptr(mat),
                  glm::value_ptr(mat) + 16,
                  _mElements);
    }

    float* Matrix4::GetValuePtr() {
        return &_mElements[0];
    }

    Matrix4 Matrix4::Translate(Matrix4 matrix, Vector3 vec) {
        glm::mat4 mat = glm::make_mat4(matrix._mElements);
        glm::vec3 v = glm::vec3(vec.X, vec.Y, vec.Z);
        glm::mat4 translated = glm::translate(mat, v);

        Matrix4 out;
        std::copy(glm::value_ptr(translated),
                  glm::value_ptr(translated) + 16,
                  out._mElements);
        return out;
    }

    Matrix4 Matrix4::Scale(Matrix4 matrix, Vector3 vec) {
        glm::mat4 mat = glm::make_mat4(matrix._mElements);
        glm::vec3 v = glm::vec3(vec.X, vec.Y, vec.Z);
        glm::mat4 scaled = glm::scale(mat, v);

        Matrix4 out;
        std::copy(glm::value_ptr(scaled),
                  glm::value_ptr(scaled) + 16,
                  out._mElements);
        return out;
    }

    Matrix4 Matrix4::ToMat4(Quaternion quat) {
        glm::quat quaternion = glm::quat(quat.W(), quat.X(), quat.Y(), quat.Z());
        glm::mat4 mat = glm::toMat4(quaternion);

        Matrix4 out;
        std::copy(glm::value_ptr(mat),
                  glm::value_ptr(mat) + 16,
                  out._mElements);
        return out;
    }

    Matrix4 Matrix4::Perspective(float fovy, float aspect, float near, float far) {
	    glm::mat4 mat = glm::perspective(fovy, aspect, near, far);
	    Matrix4 out;
        std::copy(glm::value_ptr(mat),
                  glm::value_ptr(mat) + 16,
                  out._mElements);
        return out;
    }

    Matrix4 Matrix4::Ortho(float left, float right, float bottom, float top) {
	    glm::mat4 mat = glm::ortho(left, right, bottom, top);
	    Matrix4 out;
        std::copy(glm::value_ptr(mat),
                  glm::value_ptr(mat) + 16,
                  out._mElements);
        return out;
    }

    Matrix4 Matrix4::LookAt(Vector3 eye, Vector3 center, Vector3 up) {
        glm::vec3 eye_vec = glm::vec3(eye.X, eye.Y, eye.Z);
        glm::vec3 center_vec = glm::vec3(center.X, center.Y, center.Z);
        glm::vec3 up_vec = glm::vec3(up.X, up.Y, up.Z);
        glm::mat4 mat = glm::lookAt(eye_vec, center_vec, up_vec);
        Matrix4 out;
        std::copy(glm::value_ptr(mat),
                  glm::value_ptr(mat) + 16,
                  out._mElements);
        return out;
    }

    Matrix4 Matrix4::operator*(const Matrix4& rhs) {
        glm::mat4 matLhs = glm::make_mat4(_mElements);
        glm::mat4 matRhs = glm::make_mat4(rhs._mElements);
        glm::mat4 result = matLhs * matRhs;
        Matrix4 out;
        std::copy(glm::value_ptr(result),
                  glm::value_ptr(result) + 16,
                  out._mElements);
        return out;
    }

    Vector4 Matrix4::operator*(const Vector4& rhs) {
        glm::mat4 mat = glm::make_mat4(_mElements);
        glm::vec4 vec = glm::vec4(rhs.X, rhs.Y, rhs.Z, rhs.W);
        glm::vec4 result = mat * vec;

        return Vector4{result.x, result.y, result.z, result.w};
    }

    Matrix4 Matrix4::Inverse(Matrix4 mat) {
        glm::mat4 toInv = glm::make_mat4(mat._mElements);
        glm::mat4 inv = glm::inverse(toInv);
        Matrix4 out;
        std::copy(glm::value_ptr(inv),
                  glm::value_ptr(inv) + 16,
                  out._mElements);
        return out;
    }
}