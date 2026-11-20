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
	static Ray CreateRay(Vector3 RayStart, float Distance);
    static Vector3 Radians(Vector3 eulerAngles);
    static float Radians(float degrees);
};
}