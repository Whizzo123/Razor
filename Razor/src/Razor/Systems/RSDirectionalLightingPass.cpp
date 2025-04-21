#include "RSDirectionalLightingPass.h"
#include "../Component.h"

namespace Razor
{

	void RSDirectionalLightingPass::Render(RenderPipelineEntityProperties& Properties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<DirectionalLight>();

		for (auto RenderingEntity : View)
		{
			DirectionalLight& Light = CurrentScene->GetComponent<DirectionalLight>(RenderingEntity);
			for (auto& Pair : Properties.Properties)
			{
				EntityRenderProperty& Property = Pair.second;
				for (int i = 0; i < Property.GetNumberOfSlots(); i++)
				{
					PropertySlot& Slot = Property.GetPropertySlot(i);

					Slot.AddProperty<glm::vec3>("light.position", Light.Position);
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

