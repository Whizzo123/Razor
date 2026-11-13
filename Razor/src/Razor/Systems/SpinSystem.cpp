#include "SpinSystem.h"
#include "../Component.h"

namespace Razor
{

	void SpinSystem::Run(float dt)
	{
		auto View = _mCurrentScene->GetEntitiesWithComponents<Transform>();
		for (auto Entity : View)
		{
			Transform& EntityTransform = _mCurrentScene->GetComponent<Transform>(Entity);
			Vector3 TransformRotation = EntityTransform.Rotation;
			float lastRotateX = TransformRotation.X;
			float lastRotateY = TransformRotation.Y;
			float lastRotateZ = TransformRotation.Z;
			EntityTransform.Rotate(Vector3(lastRotateX + (1.0f * dt), lastRotateY + (2.0f * dt), lastRotateZ + (4.0f * dt)));
		}
	}

}
