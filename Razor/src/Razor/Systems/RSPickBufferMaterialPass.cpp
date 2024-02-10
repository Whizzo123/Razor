#include "RSPickBufferMaterialPass.h"
#include "../Component.h"

namespace Razor
{
	void RSPickBufferMaterialPass::Render(RenderPipelineEntityProperties& Properties)
	{
		for (Entity RenderingEntity : Entities)
		{
			Properties.HighestEntity = RenderingEntity;
			Material& EntityMat = Coordinator->GetComponent<Material>(RenderingEntity);

			EntityRenderProperty& Property = Properties.Properties[RenderingEntity];
			Property.GeneratePropertySlots(EntityMat.Materials.size());
			for (int i = 0; i < EntityMat.Materials.size(); i++)
			{
				PropertySlot& Slot = Property.GetPropertySlot(i);
				//Red, Green, Blue
				Slot.AddVec3Property("entitycolor", glm::vec3((RenderingEntity >> 16) & 0xff, (RenderingEntity >> 8) & 0xff, RenderingEntity & 0xff));
			}
		}
	}
}