#include "RSTransformationsPass.h"
#include "../Component.h"

namespace Razor
{
	void RSTransformationsPass::Render(RenderPipelineEntityProperties& EntityProperties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<Transform>();
		for (auto Entity : View)
		{
			Transform& EntityTransform = CurrentScene->GetComponent<Transform>(Entity);
			if (EntityProperties.Properties.find(Entity) == EntityProperties.Properties.end())
			{
				continue;
			}
			EntityRenderProperty& EntityProperty = EntityProperties.Properties[Entity];
			for (int i = 0; i < EntityProperty.GetNumberOfSlots(); i++)
			{
				// TODO for some reason mat4 properties we add get immediately lost :) 
				EntityProperty.GetPropertySlot(i).AddProperty<glm::mat4>("model", EntityTransform.Get());
			}
		}
	}
}