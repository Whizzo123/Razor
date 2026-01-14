#pragma once
#include "../IPhysicsEngine.h"


namespace Razor
{
	struct BoxBody
	{
		unsigned int bodyId = 0xFFFFFFFF;
		bool mbUseGravity = false;
		float mMass = 1.0f;
		EPhysicsMotionType mMotionType = EPhysicsMotionType::Static;
		bool mbIsStatic = false;
		std::function<void()> OnCollisionStarted;
	};
}