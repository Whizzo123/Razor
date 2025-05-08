#include "RSEditorCamera.h"

namespace EdgeEditor
{

	void RSEditorCamera::Render(Razor::RenderPipelineEntityProperties& EntityProperties)
	{
		const int SCREEN_WIDTH = 800;
		const int SCREEN_HEIGHT = 600;

		glm::mat4 CameraProjection = glm::perspective(glm::radians(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 CameraView = glm::lookAt(EditorCamera.CameraPos, EditorCamera.CameraPos + EditorCamera.CameraFront, EditorCamera.CameraUp);
		for (auto& Pair : EntityProperties.Properties)
		{
			Razor::EntityRenderProperty& Property = Pair.second;
			for (int j = 0; j < Property.GetNumberOfSlots(); j++)
			{
				Razor::PropertySlot& Slot = Property.GetPropertySlot(j);

				Slot.AddProperty<glm::mat4>("projection", CameraProjection);
				Slot.AddProperty<glm::mat4>("view", CameraView);
				Slot.AddProperty<glm::vec3>("viewPos", EditorCamera.CameraPos);
			}
		}

	}

}