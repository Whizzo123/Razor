#include "RSDirectionalLightingPass.h"
#include "../Component.h"

namespace Razor
{

	void RSDirectionalLightingPass::Render(RenderPipelineEntityProperties& Properties)
	{
		for (Entity RenderingEntity : Entities)
		{
			DirectionalLight& Light = Coordinator->GetComponent<DirectionalLight>(RenderingEntity);
			for (int j = 0; j <= Properties.HighestEntity; j++)
			{
				EntityRenderProperty& Property = Properties.Properties[j];
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

