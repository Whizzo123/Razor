#pragma once

namespace Razor {
    struct Vector3
	{
		float X;
		float Y;
		float Z;
		static Vector3 Normalise(Vector3 vec);
		static Vector3 Cross(Vector3 a, Vector3 b);
		Vector3 operator+(const Vector3& vec);
		void operator+=(const Vector3& vec);
		Vector3 operator-(const Vector3& vec);
		void operator-=(const Vector3& vec);
		Vector3 operator*(const float rhs) const;

	};
}