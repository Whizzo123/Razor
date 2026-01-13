#include "JoltPhysicsEngine.h"

#include <Jolt/Core/Memory.h>
#include <Jolt/Core/Factory.h>
#include <Jolt/RegisterTypes.h>
#include <Jolt/Physics/PhysicsSettings.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include "JoltDebugRenderer.h"

JPH_SUPPRESS_WARNINGS

using namespace JPH::literals;

#include <thread>
#include <cstdarg>
#include <iostream>

#include "../../Utils/Vector.h"

namespace Razor
{
	// Callback for traces, connect this to your own trace function if you have one
	static void TraceImpl(const char* inFMT, ...)
	{
		// Format the message
		va_list list;
		va_start(list, inFMT);
		char buffer[1024];
		vsnprintf(buffer, sizeof(buffer), inFMT, list);
		va_end(list);

		// Print to the TTY
		std::cout << buffer << std::endl;
	}

#ifdef JPH_ENABLE_ASSERTS

	// Callback for asserts, connect this to your own assert handler if you have one
	static bool AssertFailedImpl(const char* inExpression, const char* inMessage, const char* inFile, unsigned int inLine)
	{
		// Print to the TTY
		std::cout << inFile << ":" << inLine << ": (" << inExpression << ") " << (inMessage != nullptr ? inMessage : "") << std::endl;

		// Breakpoint
		return true;
	};


#endif // JPH_ENABLE_ASSERTS

	bool ObjectLayerPairFilterImpl::ShouldCollide(JPH::ObjectLayer inObject1, JPH::ObjectLayer inObject2) const
	{
		switch (inObject1)
		{
		case Layers::NON_MOVING:
			return inObject2 == Layers::MOVING; // Non moving only collides with moving
		case Layers::MOVING:
			return true; // Moving collides with everything
		default:
			JPH_ASSERT(false);
			return false;
		}
	}

	BPLayerInterfaceImpl::BPLayerInterfaceImpl()
	{
		// Create a mapping table from object to broad phase layer
		mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
		mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
	}

	unsigned int BPLayerInterfaceImpl::GetNumBroadPhaseLayers() const
	{
		return BroadPhaseLayers::NUM_LAYERS;
	}

	JPH::BroadPhaseLayer BPLayerInterfaceImpl::GetBroadPhaseLayer(JPH::ObjectLayer inLayer) const 
	{
		JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
		return mObjectToBroadPhase[inLayer];
	}

	#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
	const char* BPLayerInterfaceImpl::GetBroadPhaseLayerName(JPH::BroadPhaseLayer inLayer) const
	{
		switch ((JPH::BroadPhaseLayer::Type)inLayer)
		{
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
		case (JPH::BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
		default:													JPH_ASSERT(false); return "INVALID";
		}
	}
	#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED



	bool ObjectVsBroadPhaseLayerFilterImpl::ShouldCollide(JPH::ObjectLayer inLayer1, JPH::BroadPhaseLayer inLayer2) const
	{
		switch (inLayer1)
		{
		case Layers::NON_MOVING:
			return inLayer2 == BroadPhaseLayers::MOVING;
		case Layers::MOVING:
			return true;
		default:
			JPH_ASSERT(false);
			return false;
		}
	}
	

	JPH::ValidateResult	MyContactListener::OnContactValidate(const JPH::Body& inBody1, const JPH::Body& inBody2, JPH::RVec3Arg inBaseOffset, const JPH::CollideShapeResult& inCollisionResult)
	{
		std::cout << "Contact validate callback" << std::endl;

		// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
		return JPH::ValidateResult::AcceptAllContactsForThisBodyPair;
	}

	void MyContactListener::OnContactAdded(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		std::cout << "A contact was added" << std::endl;
		_mBodyContactMap[inBody1.GetID()].push_back({ EContactType::Started, inBody2.GetID().GetIndex(), false });
		_mBodyContactMap[inBody2.GetID()].push_back({ EContactType::Started, inBody1.GetID().GetIndex(), false });
	}

	void MyContactListener::OnContactPersisted(const JPH::Body& inBody1, const JPH::Body& inBody2, const JPH::ContactManifold& inManifold, JPH::ContactSettings& ioSettings)
	{
		std::cout << "A contact was persisted" << std::endl;

		auto processContact = [=](JPH::BodyID first, JPH::BodyID second)
		{
			for(ContactInfo& info : _mBodyContactMap[first])
			{
				if (info.mOtherBodyId == second.GetIndex() && info.mContactProcessed)
				{
					info.mContactType = EContactType::Persisted;
					info.mContactProcessed = false;
				}
			}
		};

		processContact(inBody1.GetID(), inBody2.GetID());
		processContact(inBody2.GetID(), inBody1.GetID());
	}

	void MyContactListener::OnContactRemoved(const JPH::SubShapeIDPair& inSubShapePair)
	{
		std::cout << "A contact was removed" << std::endl;

		auto processContact = [=](JPH::BodyID first, JPH::BodyID second)
			{
				for (ContactInfo& info : _mBodyContactMap[first])
				{
					if (info.mOtherBodyId == second.GetIndex() && info.mContactProcessed)
					{
						info.mContactType = EContactType::Ended;
						info.mContactProcessed = false;
					}
				}
			};
		processContact(inSubShapePair.GetBody1ID(), inSubShapePair.GetBody2ID());
		processContact(inSubShapePair.GetBody2ID(), inSubShapePair.GetBody1ID());
	}

	void MyBodyActivationListener::OnBodyActivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData)
	{
		std::cout << "A body got activated" << std::endl;
	}

	void MyBodyActivationListener::OnBodyDeactivated(const JPH::BodyID& inBodyID, uint64_t inBodyUserData)
	{
		std::cout << "A body went to sleep" << std::endl;
	}

	JoltPhysicsEngine::JoltPhysicsEngine(Ref<JoltDebugRenderer> debugRenderer) :  IPhysicsEngine()
	{
		// Register allocation hook. Default to malloc / free.
		JPH::RegisterDefaultAllocator();

		// Install trace and assert callbacks
		JPH::Trace = TraceImpl;
		JPH_IF_ENABLE_ASSERTS(JPH::AssertFailed = AssertFailedImpl);

		// Responsible for creating instances of classes based on their name or hash mainly for saved data
		JPH::Factory::sInstance = new JPH::Factory();

		// Register all physics types with the factory and install their collision handlers
		JPH::RegisterTypes();

		_mTempAllocator = CreateScope<JPH::TempAllocatorImpl>(10 * 1024 * 1024); // 10 MB

		_mJobSystem = CreateScope<JPH::JobSystemThreadPool>(1024, 8, std::thread::hardware_concurrency() - 1);

		JPH::DebugRenderer::sInstance = debugRenderer.get();

		// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an error.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		const unsigned int cMaxBodies = 1024;

		// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the default settings.
		const unsigned int cNumBodyMutexes = 0;

		// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
		// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this buffer
		// too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly less efficient.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 65536.
		const unsigned int cMaxBodyPairs = 65536;

		// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected than this
		// number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
		// Note: This value is low because this is a simple test. For a real project use something in the order of 10240.
		const unsigned int cMaxContactConstraints = 10240;

		// Create mapping table from object layer to broadphase layer
		// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		// Also have a look at BroadPhaseLayerInterfaceTable or BroadPhaseLayerInterfaceMask for a simpler interface.
		_mBroadPhaseLayerInterface = CreateScope<BPLayerInterfaceImpl>();

		// Create class that filters object vs broadphase layers
		// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		// Also have a look at ObjectVsBroadPhaseLayerFilterTable or ObjectVsBroadPhaseLayerFilterMask for a simpler interface.
		_mObjectVsBroadphaseLayerFilter = CreateScope<ObjectVsBroadPhaseLayerFilterImpl>();

		// Create class that filters object vs object layers
		// Note: As this is an interface, PhysicsSystem will take a reference to this so this instance needs to stay alive!
		// Also have a look at ObjectLayerPairFilterTable or ObjectLayerPairFilterMask for a simpler interface.
		_mObjectVsObjectLayerFilter = CreateScope<ObjectLayerPairFilterImpl>();

		// Now we can create the actual physics system.
		_mPhysicsSystem.Init(cMaxBodies, cNumBodyMutexes, cMaxBodyPairs, cMaxContactConstraints, *_mBroadPhaseLayerInterface, *_mObjectVsBroadphaseLayerFilter, *_mObjectVsObjectLayerFilter);
		_mPhysicsSystem.SetGravity(JPH::Vec3(0, -1, 0));

		// A body activation listener gets notified when bodies activate and go to sleep
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.
		
		_mPhysicsSystem.SetBodyActivationListener(&_mBodyActivationListener);

		// A contact listener gets notified when bodies (are about to) collide, and when they separate again.
		// Note that this is called from a job so whatever you do here needs to be thread safe.
		// Registering one is entirely optional.
		_mPhysicsSystem.SetContactListener(&_mContactListener);

		// Optional step: Before starting the physics simulation you can optimize the broad phase. This improves collision detection performance (it's pointless here because we only have 2 bodies).
		// You should definitely not call this every frame or when e.g. streaming in a new level section as it is an expensive operation.
		// Instead insert all new objects in batches instead of 1 at a time to keep the broad phase efficient.
	}

	JoltPhysicsEngine::~JoltPhysicsEngine()
	{
		// Unregisters all types with the factory and cleans up the default material
		JPH::UnregisterTypes();

		// Destroy the factory
		delete JPH::Factory::sInstance;
		JPH::Factory::sInstance = nullptr;
	}

	void JoltPhysicsEngine::Simulate(float deltatime)
	{
		// If you take larger steps than 1 / 60th of a second you need to do multiple collision steps in order to keep the simulation stable. Do 1 collision step per 1 / 60th of a second (round up).
		const int cCollisionSteps = 1;

		accumulator += deltatime;

		const float fixedStep = 1.0f / 30.0f;

		if (accumulator >= fixedStep)
		{
			// Step the world
			_mPhysicsSystem.Update(fixedStep, cCollisionSteps, _mTempAllocator.get(), _mJobSystem.get());
			accumulator -= fixedStep;
		}
	}

	Vector3 JoltPhysicsEngine::GetPosition(unsigned int bodyId) const
	{
		const JPH::BodyInterface& interface = _mPhysicsSystem.GetBodyInterface();
		JPH::RVec3 position = interface.GetCenterOfMassPosition(JPH::BodyID(bodyId));

		JPH::BodyLockRead lock(_mPhysicsSystem.GetBodyLockInterface(), JPH::BodyID(bodyId));
		if (!lock.Succeeded())
			return Vector3{ position.GetX(), position.GetY(), position.GetZ() };

		const JPH::Body& body = lock.GetBody();

		JPH::AABox aabb = body.GetWorldSpaceBounds();

		JPH::DebugRenderer::sInstance->DrawWireBox(aabb, JPH::Color::sRed);

		return Vector3{ position.GetX(), position.GetY(), position.GetZ() };
	}

	void JoltPhysicsEngine::ApplyLinearVelocity(unsigned int bodyId, const Vector3& velocity)
	{
		JPH::BodyInterface& interface = _mPhysicsSystem.GetBodyInterface();
		JPH::Vec3 jphVelocity{ velocity.X, velocity.Y, velocity.Z };
		_mBodyInterface->SetLinearVelocity(JPH::BodyID(bodyId), jphVelocity);
	}
	
	unsigned int JoltPhysicsEngine::CreateBoxRigidBody(Vector3 position, float mass, EPhysicsMotionType motionType, bool bIsStatic)
	{
		JPH::BodyInterface& interface = _mPhysicsSystem.GetBodyInterface();

		// Next we can create a rigid body to serve as the floor, we make a large box
		// Create the settings for the collision volume (the shape).
		// Note that for simple shapes (like boxes) you can also directly construct a BoxShape.
		JPH::BoxShapeSettings floor_shape_settings(JPH::Vec3(1.0f, 1.0f, 1.0f));
		floor_shape_settings.SetEmbedded(); // A ref counted object on the stack (base class RefTarget) should be marked as such to prevent it from being freed when its reference count goes to 0.

		// Create the shape
		JPH::ShapeSettings::ShapeResult floor_shape_result = floor_shape_settings.Create();
		JPH::ShapeRefC floor_shape = floor_shape_result.Get(); // We don't expect an error here, but you can check floor_shape_result for HasError() / GetError()

		// Create the settings for the body itself. Note that here you can also set other properties like the restitution / friction.
		JPH::ObjectLayer layer = Layers::MOVING;
		if (bIsStatic)
		{
			layer = Layers::NON_MOVING;
		}
		JPH::EMotionType motion = JPH::EMotionType::Static;
		switch (motionType)
		{
		case (EPhysicsMotionType::Kinematic):
			motion = JPH::EMotionType::Kinematic;
			break;
		case (EPhysicsMotionType::Dynamic):
			motion = JPH::EMotionType::Dynamic;
			break;
		default:
			motion = JPH::EMotionType::Static;
		}
		JPH::BodyCreationSettings floor_settings(floor_shape, JPH::RVec3(position.X, position.Y, position.Z), JPH::Quat::sIdentity(), motion, layer);
		JPH::MassProperties massOverride;
		massOverride.ScaleToMass(mass);
		floor_settings.mMassPropertiesOverride = massOverride;

		// Create the actual rigid body
		JPH::Body* floor = interface.CreateBody(floor_settings); // Note that if we run out of bodies this can return nullptr

		// Add it to the world
		interface.AddBody(floor->GetID(), JPH::EActivation::Activate);

		_mPhysicsSystem.OptimizeBroadPhase();

		return floor->GetID().GetIndexAndSequenceNumber();
	}

	void JoltPhysicsEngine::SetGravity(unsigned int bodyId, bool useGravity)
	{
		JPH::BodyInterface& interface = _mPhysicsSystem.GetBodyInterface();
		if (useGravity)
		{
			interface.SetGravityFactor(static_cast<JPH::BodyID>(bodyId), 1.0f);
		}
		else
		{
			interface.SetGravityFactor(static_cast<JPH::BodyID>(bodyId), 0.0f);
		}
	}

	void JoltPhysicsEngine::DestroyBody(unsigned int bodyId)
	{
		JPH::BodyInterface& interface = _mPhysicsSystem.GetBodyInterface();
		interface.RemoveBody(static_cast<JPH::BodyID>(bodyId));
		interface.DestroyBody(static_cast<JPH::BodyID>(bodyId));
	}

	std::vector<ContactInfo> JoltPhysicsEngine::GetContactInfo(unsigned int bodyId)
	{
		std::vector<ContactInfo> ret;
		if (_mContactListener._mBodyContactMap.find(JPH::BodyID(bodyId)) != _mContactListener._mBodyContactMap.end())
		{
			ret = _mContactListener._mBodyContactMap[static_cast<JPH::BodyID>(bodyId)];

			std::vector<ContactInfo> clearList;
			for (ContactInfo& info : _mContactListener._mBodyContactMap[static_cast<JPH::BodyID>(bodyId)])
			{
				info.mContactProcessed = true;
				if (info.mContactType == EContactType::Ended)
				{
					clearList.push_back(info);
				}
			}
			for (const ContactInfo& info : clearList)
			{
				_mContactListener._mBodyContactMap[static_cast<JPH::BodyID>(bodyId)].erase(std::find(_mContactListener._mBodyContactMap[static_cast<JPH::BodyID>(bodyId)].begin(),
					_mContactListener._mBodyContactMap[static_cast<JPH::BodyID>(bodyId)].end(), info));
			}
		}
		
		return ret;
	}
}