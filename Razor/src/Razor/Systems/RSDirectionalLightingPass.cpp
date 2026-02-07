#include "RSDirectionalLightingPass.h"
#include "../Component.h"

namespace Razor
{

	void RSDirectionalLightingPass::Render(RenderPipelineData& data)
	{
		for (auto RenderingEntity : CurrentScene->GetEntitiesWithComponents<DirectionalLight, Transform>())
		{
			DirectionalLight& Light = CurrentScene->GetComponent<DirectionalLight>(RenderingEntity);
			Transform& transform = CurrentScene->GetComponent<Transform>(RenderingEntity);
			for (auto& Pair : data.mEntityRenderProperties.Properties)
			{
				ShaderProperty& Property = Pair.second;
				for (int i = 0; i < Property.GetNumberOfSlots(); i++)
				{
					ShaderPropertySlot& Slot = Property.GetPropertySlot(i);

					Slot.AddProperty<glm::vec3>("light.position", transform.Position);
					Slot.AddProperty<glm::vec3>("light.ambient", Light.Ambient);
					Slot.AddProperty<glm::vec3>("light.diffuse", Light.Diffuse);
					Slot.AddProperty<glm::vec3>("light.specular", Light.Specular);
					Slot.AddProperty<glm::vec3>("light.direction", Light.Direction);
					Slot.AddProperty<int>("light.lightType", 0);
				}
			}
		}
	}
}

