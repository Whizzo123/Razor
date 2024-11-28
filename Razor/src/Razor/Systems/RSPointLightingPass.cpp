#include "RSPointLightingPass.h"
#include "../Component.h"

namespace Razor
{
	void RSPointLightingPass::Render(RenderPipelineEntityProperties& Properties)
	{
		for (Entity RenderingEntity : Entities)
		{
			PointLight& Light = Coordinator->GetComponent<PointLight>(RenderingEntity);
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
					Slot.AddProperty<float>("light.constant", Light.Constant);
					Slot.AddProperty<float>("light.linear", Light.Linear);
					Slot.AddProperty<float>("light.quadratic", Light.Quadratic);
					Slot.AddProperty<int>("light.lightType", 1);
				}
			}
		}
	}
}