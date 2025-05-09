#include "SpinSystem.h"
#include "../Component.h"

namespace Razor
{

	void SpinSystem::Run(float dt)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<Transform>();
		for (auto Entity : View)
		{
			Transform& EntityTransform = CurrentScene->GetComponent<Transform>(Entity);
			glm::vec3 TransformRotation = EntityTransform.Rotation;
			float lastRotateX = TransformRotation.x;
			float lastRotateY = TransformRotation.y;
			float lastRotateZ = TransformRotation.z;
			EntityTransform.Rotate(glm::vec3(lastRotateX + (1.0f * dt), lastRotateY + (2.0f * dt), lastRotateZ + (4.0f * dt)));
		}
	}

}
