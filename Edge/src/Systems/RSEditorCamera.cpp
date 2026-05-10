#include "RSEditorCamera.h"

namespace EdgeEditor
{

	void RSEditorCamera::Render(Razor::RenderPipelineData& data)
	{
		const float AspectRatio = ViewportHeight > 0 ? (float)ViewportWidth / (float)ViewportHeight : 1.0f;
		glm::mat4 CameraProjection = glm::perspective(glm::radians(45.0f), AspectRatio, 0.1f, 100.0f);
		glm::mat4 CameraView = glm::lookAt(EditorCamera.CameraPos, EditorCamera.CameraPos + EditorCamera.CameraFront, EditorCamera.CameraUp);
		for (auto& Pair : data.mEntityRenderProperties.Properties)
		{
			Razor::ShaderProperty& Property = Pair.second;
			for (int j = 0; j < Property.GetNumberOfSlots(); j++)
			{
				Razor::ShaderPropertySlot& Slot = Property.GetPropertySlot(j);

				Slot.AddProperty<glm::mat4>("projection", CameraProjection);
				Slot.AddProperty<glm::mat4>("view", CameraView);
				Slot.AddProperty<glm::vec3>("viewPos", EditorCamera.CameraPos);
			}
		}

	}

}