#include "SpinSystem.h"
#include "../Component.h"

void SpinSystem::Run(float dt)
{
	for (Entity Entity : Entities)
	{
		Transform& EntityTransform = Coordinator->GetComponent<Transform>(Entity);
		glm::vec3 TransformRotation = EntityTransform.GetRotation();
		float lastRotateX = TransformRotation.x;
		float lastRotateY = TransformRotation.y;
		float lastRotateZ = TransformRotation.z;
		EntityTransform.Rotate(glm::vec3(lastRotateX + (1.0f * dt), lastRotateY + (2.0f * dt), lastRotateZ + (4.0f * dt)));
	}
}
