#include "RSTransformationsPass.h"
#include "../Component.h"

namespace Razor
{
	void RSTransformationsPass::Render(RenderPipelineEntityProperties& EntityProperties)
	{
		for (const Entity& Entity : Entities)
		{
			Transform& EntityTransform = Coordinator->GetComponent<Transform>(Entity);
			EntityRenderProperty& EntityProperty = EntityProperties.Properties[Entity];
			for (int i = 0; i < EntityProperty.GetNumberOfSlots(); i++)
			{
				// TODO for some reason mat4 properties we add get immediately lost :) 
				EntityProperty.GetPropertySlot(i).AddProperty<glm::mat4>("model", EntityTransform.Get());
			}
		}
	}
}