#include "RSCameraPass.h"

namespace Razor
{
	void RSCameraPass::Render(RenderPipelineData& data)
	{
		const float AspectRatio = ViewportHeight > 0 ? (float)ViewportWidth / (float)ViewportHeight : 1.0f;

		auto View = CurrentScene->GetEntitiesWithComponents<Camera>();

		glm::mat4 CameraProjection = glm::mat4(1.0f);
		glm::mat4 CameraView = glm::mat4(1.0f);
		glm::vec3 CameraPos = glm::vec3(0.0f);

		for(auto CameraEntity : View)
		{
			Camera& GameCamera = CurrentScene->GetComponent<Camera>(CameraEntity);
			Transform& CameraTransform = CurrentScene->GetComponent<Transform>(CameraEntity);
			CameraProjection = glm::perspective(glm::radians(45.0f), AspectRatio, 0.1f, 100.0f);
			CameraView = glm::lookAt(CameraTransform.Position, CameraTransform.Position + GameCamera.CameraFront, GameCamera.CameraUp);
			CameraPos = CameraTransform.Position;

			for (auto& Pair : data.mEntityRenderProperties.Properties)
			{
				ShaderProperty& Property = Pair.second;
				for (int j = 0; j < Property.GetNumberOfSlots(); j++)
				{
					ShaderPropertySlot& Slot = Property.GetPropertySlot(j);

					Slot.AddProperty<glm::mat4>("projection", CameraProjection);
					Slot.AddProperty<glm::mat4>("view", CameraView);
					Slot.AddProperty<glm::vec3>("viewPos", CameraTransform.Position);
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