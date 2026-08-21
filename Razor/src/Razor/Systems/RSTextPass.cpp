#include "RSTextPass.h"
#include "../Renderer/Font/Text.h"

namespace Razor
{
    void RSTextPass::Render(RenderPipelineData& data)
    {
        auto View = _mCurrentScene->GetEntitiesWithComponents<Text>();
		for (auto RenderingEntity : View)
		{
			Text& EntityText = _mCurrentScene->GetComponent<Text>(RenderingEntity);
			
			if (data.mEntityRenderProperties.Properties.find(RenderingEntity) == data.mEntityRenderProperties.Properties.end())
			{
				data.mEntityRenderProperties.Properties.insert(std::pair(RenderingEntity, ShaderProperty()));
			}
			ShaderProperty& Property = data.mEntityRenderProperties.Properties[RenderingEntity];
			Property.GeneratePropertySlots(1);
			for (int i = 0; i < 1; i++)
			{
				ShaderPropertySlot& Slot = Property.GetPropertySlot(i);
				Slot.AddProperty<glm::vec3>("text_color", glm::vec3 {EntityText.mColor.X, EntityText.mColor.Y, EntityText.mColor.Z});
			}
		}
    }
}