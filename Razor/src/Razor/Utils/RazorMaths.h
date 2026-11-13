#pragma once
#include "Matrix4.h"
#include "Vector3.h"
#include "Vector2.h"

namespace Razor {

struct Ray
{
	Vector3 Start;
	Vector3 End;
};

class RazorMaths
{
public:
	static Vector2 ScreenToView(Vector2 ScreenPos, int ScreenWidth, int ScreenHeight);
	//static Vector3 ViewToWorld(Vector2 ViewPos, Matrix4 ViewMatrix);
	static Ray CreateRay(Vector3 RayStart, float Distance);
    static Vector3 Radians(Vector3 eulerAngles);
    static float Radians(float degrees);
};
}

// TODO create functionality for a ray-object intersection test
/*bool intersect(const Ray& ray, float& t) const {
        glm::vec3 oc = ray.origin - center;
        float a = glm::dot(ray.direction, ray.direction);
        float b = 2.0f * glm::dot(oc, ray.direction);
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            // No intersection
            return false;
        }

        // Compute the closest intersection point
        t = (-b - sqrt(discriminant)) / (2.0f * a);
        return true;
    }
    
    if (sphere.intersect(ray, t)) {
        // Intersection occurred at distance t along the ray
        // Do something with the intersection point
        glm::vec3 intersectionPoint = ray.origin + t * ray.direction;
    } else {
        // No intersection
    }*/