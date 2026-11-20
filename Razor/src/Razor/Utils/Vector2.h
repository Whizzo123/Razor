#pragma once

namespace Razor
{
	struct Vector2
	{
		float X;
		float Y;
		static Vector2 Normalise(Vector2 vec);
		Vector2 operator-(const Vector2& vec);
		
	};
}