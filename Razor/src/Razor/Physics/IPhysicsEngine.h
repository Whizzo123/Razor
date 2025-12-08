#pragma once

namespace Razor
{
	class Vector3;

	class IPhysicsEngine
	{
	public:
		virtual ~IPhysicsEngine() = default;
		virtual void Simulate(float deltaTime) = 0;
		virtual Vector3 GetPosition(unsigned int bodyId) const = 0;
		virtual void ApplyLinearVelocity(unsigned int bodyId, const Vector3& velocity) = 0;
		virtual unsigned int CreateBoxRigidBody(Vector3 position) = 0;
		virtual void SetGravity(unsigned int bodyId, bool useGravity) = 0;
		virtual void DestroyBody(unsigned int bodyId) = 0;
	};
}

