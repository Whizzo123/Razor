#pragma once
#include "../IPhysicsEngine.h"

#include <Jolt/Jolt.h>
#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>
#include <Jolt/Physics/Body/BodyActivationListener.h>
#include <Jolt/Physics/Collision/ContactListener.h>
#include "../../Core.h"


namespace Razor
{
	class JoltDebugRenderer;
	// Each broadphase layer results in a separate bounding volume tree in the broad phase. You at least want to have
	// a layer for non-moving and moving objects to avoid having to update a tree full of static objects every frame.
	// You can have a 1-on-1 mapping between object layers and broadphase layers (like in this case) but if you have
	// many object layers you'll be creating many broad phase trees, which is not efficient. If you want to fine tune
	// your broadphase layers define JPH_TRACK_BROADPHASE_STATS and look at the stats reported on the TTY.
	namespace BroadPhaseLayers
	{
		static constexpr JPH::BroadPhaseLayer NON_MOVING(0);
		static constexpr JPH::BroadPhaseLayer MOVING(1);
		static constexpr unsigned int NUM_LAYERS(2);
	};

	// Layer that objects can be in, determines which other objects it can collide with
	// Typically you at least want to have 1 layer for moving bodies and 1 layer for static bodies, but you can have more
	// layers if you want. E.g. you could have a layer for high detail collision (which is not used by the physics simulation
	// but only if you do collision testing).
	namespace Layers
	{
		static constexpr JPH::ObjectLayer NON_MOVING = 0;
		static constexpr JPH::ObjectLayer MOVING = 1;
		static constexpr JPH::ObjectLayer NUM_LAYERS = 2;
	};

	/// Class that determines if two object layers can collide
	class ObjectLayerPairFilterImpl : public JPH::ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const override;
	};

	class BPLayerInterfaceImpl final : public JPH::BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl();

		virtual unsigned int GetNumBroadPhaseLayers() const override;

		virtual JPH::BroadPhaseLayer GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const override;

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const override;
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		JPH::BroadPhaseLayer					mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public JPH::ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const override;
	};

	// An example contact listener
	class MyContactListener : public JPH::ContactListener
	{
	public:
		// See: ContactListener
		virtual JPH::ValidateResult	OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult) override;

		virtual void OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;

		virtual void OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings) override;

		virtual void OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair) override;

		friend class JoltPhysicsEngine;
	private:
		std::unordered_map<JPH::BodyID, std::vector<ContactInfo>> _mBodyContactMap;
	};

	// An example activation listener
	class MyBodyActivationListener : public JPH::BodyActivationListener
	{
	public:
		virtual void OnBodyActivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData) override;

		virtual void OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData) override;
	};

	class JoltPhysicsEngine : public IPhysicsEngine
	{
	public:
		JoltPhysicsEngine(Ref<JoltDebugRenderer> debugRenderer);
		~JoltPhysicsEngine() override;
		void Simulate(float deltaTime) override;
		Vector3 GetPosition(unsigned int bodyId) const override;
		void ApplyLinearVelocity(unsigned int bodyId, const Vector3& velocity) override;
		unsigned int CreateBoxRigidBody(Vector3 position, float mass, EPhysicsMotionType motionType, bool bIsStatic) override;
		void SetGravity(unsigned int bodyId, bool useGravity) override;
		void DestroyBody(unsigned int bodyId) override;
		std::vector<ContactInfo> GetContactInfo(unsigned int bodyId) override;

	private:
		JPH::PhysicsSystem _mPhysicsSystem;
		// Memory for temp allocation during physics update ~10Mb
		Scope<JPH::TempAllocatorImpl> _mTempAllocator;
		// We need a job system that will execute physics jobs on multiple threads. Typically
		// you would implement the JobSystem interface yourself and let Jolt Physics run on top
		// of your own job scheduler. JobSystemThreadPool is an example implementation.
		Scope<JPH::JobSystemThreadPool> _mJobSystem;
		
		Scope<BPLayerInterfaceImpl> _mBroadPhaseLayerInterface;
		Scope<ObjectVsBroadPhaseLayerFilterImpl> _mObjectVsBroadphaseLayerFilter;
		Scope<ObjectLayerPairFilterImpl> _mObjectVsObjectLayerFilter;

		std::shared_ptr<JPH::BodyInterface> _mBodyInterface;

		MyBodyActivationListener _mBodyActivationListener;
		MyContactListener _mContactListener;		

		float accumulator = 0.0f;
	};
}

