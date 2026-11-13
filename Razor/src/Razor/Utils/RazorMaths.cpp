#include "RazorMaths.h"
#include "Vector4.h"
#include <glm/glm.hpp>

namespace Razor {

	Vector2 RazorMaths::ScreenToView(Vector2 ScreenPos, int ScreenWidth, int ScreenHeight)
	{
		return Vector2((2.0f / ScreenPos.X) / ScreenWidth - 1.0f, 1.0f - (2.0f * ScreenPos.Y) / ScreenHeight);
	}

	/*Vector3 RazorMaths::ViewToWorld(Vector2 ViewPos, Matrix4 ViewMatrix)
	{
		//Matrix4 InvViewMatrix = Matrix4::Inverse(ViewMatrix);
		//Vector4 WorldPos = InvViewMatrix * Vector4(ViewPos.X, ViewPos.Y, 0.0f, 1.0f);
		return Vector3();
	}*/

	Ray RazorMaths::CreateRay(Vector3 RayStart, float Distance)
	{
		Ray NewRay;
		NewRay.Start = RayStart;
		NewRay.End = Vector3(RayStart.X, RayStart.Y, RayStart.Z * Distance);
		return NewRay;
	}

	Vector3 RazorMaths::Radians(Vector3 eulerAngles)
	{
		glm::vec3 vec = glm::vec3(eulerAngles.X, eulerAngles.Y, eulerAngles.Z);
		glm::vec3 rad = glm::radians(vec);
		return Vector3(rad.x, rad.y, rad.z);
	}

	float RazorMaths::Radians(float degrees)
	{
		return glm::radians(degrees);
	}

}
