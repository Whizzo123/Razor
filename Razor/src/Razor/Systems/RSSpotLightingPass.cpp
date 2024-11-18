#include "RSSpotLightingPass.h"
#include "../Component.h"

namespace Razor
{
	void RSSpotLightingPass::Render(RenderPipelineEntityProperties& Properties)
	{
		for (Entity RenderingEntity : Entities)
		{
			SpotLight& Light = Coordinator->GetComponent<SpotLight>(RenderingEntity);
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
					Slot.AddFloatProperty("light.cutoff", Light.Cutoff);
					Slot.AddFloatProperty("light.outerCutoff", Light.OuterCutoff);
					Slot.AddIntProperty("light.lightType", 2);
				}
			}
		}
	}
}