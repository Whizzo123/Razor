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

					Slot.AddVec3Property("light.position", Light.Position);
					Slot.AddVec3Property("light.ambient", Light.Ambient);
					Slot.AddVec3Property("light.diffuse", Light.Diffuse);
					Slot.AddVec3Property("light.specular", Light.Specular);
					Slot.AddFloatProperty("light.constant", Light.Constant);
					Slot.AddFloatProperty("light.linear", Light.Linear);
					Slot.AddFloatProperty("light.quadratic", Light.Quadratic);
					Slot.AddIntProperty("light.lightType", 1);
				}
			}
		}
	}
}