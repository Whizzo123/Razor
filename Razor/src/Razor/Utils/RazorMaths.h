#pragma once
#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float4.hpp>
#include <glm/ext/matrix_float4x4.hpp>

struct Ray
{
	glm::vec3 Start;
	glm::vec3 End;
};

class RazorMaths
{
	static glm::vec2 ScreenToView(glm::vec2 ScreenPos, int ScreenWidth, int ScreenHeight);
	static glm::vec3 ViewToWorld(glm::vec2 ViewPos, glm::mat4 ViewMatrix);
	static Ray CreateRay(glm::vec3 RayStart, float Distance);
};

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