#include "RazorMaths.h"


glm::vec2 RazorMaths::ScreenToView(glm::vec2 ScreenPos, int ScreenWidth, int ScreenHeight)
{
	return glm::vec2((2.0f / ScreenPos.x) / ScreenWidth - 1.0f, 1.0f - (2.0f * ScreenPos.y) / ScreenHeight);
}

glm::vec3 RazorMaths::ViewToWorld(glm::vec2 ViewPos, glm::mat4 ViewMatrix)
{
	glm::mat4 InvViewMatrix = glm::inverse(ViewMatrix);
	glm::vec4 WorldPos = InvViewMatrix * glm::vec4(ViewPos.x, ViewPos.y, 0.0f, 1.0f);
	return WorldPos /= WorldPos.w;
}

Ray RazorMaths::CreateRay(glm::vec3 RayStart, float Distance)
{
	Ray NewRay;
	NewRay.Start = RayStart;
	NewRay.End = glm::vec3(RayStart.x, RayStart.y, RayStart.z * Distance);
	return NewRay;
}
