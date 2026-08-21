#include "RSTransformationsPass.h"
#include "../Component.h"

namespace Razor
{
	void RSTransformationsPass::Render(RenderPipelineData& data)
	{
		auto View = _mCurrentScene->GetEntitiesWithComponents<Transform>();
		for (auto Entity : View)
		{
			Transform& EntityTransform = _mCurrentScene->GetComponent<Transform>(Entity);
			if (data.mEntityRenderProperties.Properties.find(Entity) == data.mEntityRenderProperties.Properties.end())
			{
				continue;
			}
			ShaderProperty& EntityProperty = data.mEntityRenderProperties.Properties[Entity];
			for (int i = 0; i < EntityProperty.GetNumberOfSlots(); i++)
			{
				// TODO for some reason mat4 properties we add get immediately lost :) 
				EntityProperty.GetPropertySlot(i).AddProperty<glm::mat4>("model", EntityTransform.Get());
			}
		}
	}
}