#include "RSCameraPass.h"

namespace Razor
{
	void RSCameraPass::Render(RenderPipelineEntityProperties& EntityProperties)
	{
		const int SCREEN_WIDTH = 800;
		const int SCREEN_HEIGHT = 600;

		auto View = CurrentScene->GetEntitiesWithComponents<Camera>();

		for(auto CameraEntity : View)
		{
			Camera& GameCamera = CurrentScene->GetComponent<Camera>(CameraEntity);
			glm::mat4 CameraProjection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			glm::mat4 CameraView = glm::lookAt(GameCamera.CameraPos, GameCamera.CameraPos + GameCamera.CameraFront, GameCamera.CameraUp);
			for (auto& Pair : EntityProperties.Properties)
			{
				EntityRenderProperty& Property = Pair.second;
				for (int j = 0; j < Property.GetNumberOfSlots(); j++)
				{
					PropertySlot& Slot = Property.GetPropertySlot(j);

					Slot.AddProperty<glm::mat4>("projection", CameraProjection);
					Slot.AddProperty<glm::mat4>("view", CameraView);
					Slot.AddProperty<glm::vec3>("viewPos", GameCamera.CameraPos);
				}
			}
		}

	}
}