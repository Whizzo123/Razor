#pragma once

namespace Razor {
    
    struct Vector4
	{
		float X;
		float Y;
		float Z;
		float W;
		Vector4 operator/(const float rhs);
		void operator/=(const float rhs);
		void Normalise();
	};
}