#pragma once

namespace Razor
{
	struct Vector2
	{
		float X;
		float Y;
		static Vector2 Normalise(Vector2 vec);
		static Vector2 Cross(Vector2 a, Vector2 b);
		Vector2 operator-(const Vector2& vec);
		
	};
}