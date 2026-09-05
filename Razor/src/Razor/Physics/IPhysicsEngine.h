#pragma once

#include <vector>

namespace Razor
{
	struct Vector3;

	enum class EPhysicsMotionType
	{
		Static,						///< Non movable
		Kinematic,					///< Movable using velocities only, does not respond to forces
		Dynamic,					///< Responds to forces as a normal physics object
	};

	enum class EContactType
	{
		Started,
		Persisted,
		Ended
	};

	struct ContactInfo
	{
		EContactType mContactType;
		unsigned int mOtherBodyId;
		bool mContactProcessed;
		std::vector<Vector3> mCollisionPoints;
		// TODO: extend with normal and penetration depth from JPH::ContactManifold when needed

		bool operator==(const ContactInfo& other) const
		{
			return mOtherBodyId == other.mOtherBodyId;
		}
	};

	class IPhysicsEngine
	{
	public:
		virtual ~IPhysicsEngine() = default;
		virtual void Simulate(float deltaTime) = 0;
		virtual Vector3 GetPosition(unsigned int bodyId) const = 0;
		virtual void ApplyLinearVelocity(unsigned int bodyId, const Vector3& velocity) = 0;
		virtual unsigned int CreateBoxRigidBody(Vector3 position, Vector3 scale, float mass, EPhysicsMotionType motionType, bool bIsStatic, bool bIsTrigger) = 0;
		virtual void SetGravity(unsigned int bodyId, bool useGravity) = 0;
		virtual void DestroyBody(unsigned int bodyId) = 0;
		virtual std::vector<ContactInfo> GetContactInfo(unsigned int bodyId) = 0;
		virtual void MoveKinematic(unsigned int bodyId, Vector3 position, float deltaTime) = 0;
	};
}

