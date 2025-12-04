#pragma once

namespace Razor
{
	class IPhysicsEngine
	{
	public:
		virtual ~IPhysicsEngine() = default;
		virtual void Simulate(float deltaTime) = 0;
	};
}

