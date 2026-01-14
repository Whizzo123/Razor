#include "RSCameraPass.h"

namespace Razor
{
	void RSCameraPass::Render(RenderPipelineData& data)
	{
		const int SCREEN_WIDTH = 800;
		const int SCREEN_HEIGHT = 600;

		auto View = CurrentScene->GetEntitiesWithComponents<Camera>();

		
		glm::mat4 CameraProjection;
		glm::mat4 CameraView;
		glm::vec3 CameraPos;

		for(auto CameraEntity : View)
		{
			Camera& GameCamera = CurrentScene->GetComponent<Camera>(CameraEntity);
			CameraProjection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
			CameraView = glm::lookAt(GameCamera.CameraPos, GameCamera.CameraPos + GameCamera.CameraFront, GameCamera.CameraUp);
			CameraPos = GameCamera.CameraPos;

			for (auto& Pair : data.mEntityRenderProperties.Properties)
			{
				ShaderProperty& Property = Pair.second;
				for (int j = 0; j < Property.GetNumberOfSlots(); j++)
				{
					ShaderPropertySlot& Slot = Property.GetPropertySlot(j);

					Slot.AddProperty<glm::mat4>("projection", CameraProjection);
					Slot.AddProperty<glm::mat4>("view", CameraView);
					Slot.AddProperty<glm::vec3>("viewPos", GameCamera.CameraPos);
				}
			}
		}

		for (auto& property : data.mDebugLineProperties)
		{
			for (int j = 0; j < property.GetNumberOfSlots(); j++)
			{
				ShaderPropertySlot& slot = property.GetPropertySlot(j);

				slot.AddProperty<glm::mat4>("projection", CameraProjection);
				slot.AddProperty<glm::mat4>("view", CameraView);
				slot.AddProperty<glm::vec3>("viewPos", CameraPos);
			}
		}

	}
}