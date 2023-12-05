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

					Slot.AddVec3Property("light.position", Light.Position);
					Slot.AddVec3Property("light.ambient", Light.Ambient);
					Slot.AddVec3Property("light.diffuse", Light.Diffuse);
					Slot.AddVec3Property("light.specular", Light.Specular);
					Slot.AddVec3Property("light.direction", Light.Direction);
					Slot.AddIntProperty("light.lightType", 0);
				}
			}
		}
	}
}

