#include "RSCameraPass.h"

namespace Razor
{
	void RSCameraPass::Render(RenderPipelineEntityProperties& EntityProperties)
	{
		const int SCREEN_WIDTH = 800;
		const int SCREEN_HEIGHT = 600;
		for(Entity CameraEntity : Entities)
		{
			Camera& GameCamera = Coordinator->GetComponent<Camera>(CameraEntity);
			glm::mat4 View = glm::mat4(1.0f);
			glm::mat4 Projection = glm::mat4(1.0f);
			glm::mat4 CameraProjection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			glm::mat4 CameraView = glm::lookAt(GameCamera.CameraPos, GameCamera.CameraPos + GameCamera.CameraFront, GameCamera.CameraUp);
			for (int i = 0; i <= EntityProperties.HighestEntity; i++)
			{
				EntityRenderProperty& EntityProperty = EntityProperties.Properties[i];
				for (int j = 0; j < EntityProperty.GetNumberOfSlots(); j++)
				{
					PropertySlot& Slot = EntityProperty.GetPropertySlot(j);

					Slot.AddMat4Property("projection", CameraProjection);
					Slot.AddMat4Property("view", CameraView);
					Slot.AddVec3Property("viewPos", GameCamera.CameraPos);
				}
			}
		}

	}
}