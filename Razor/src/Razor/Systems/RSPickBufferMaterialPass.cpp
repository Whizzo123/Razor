#include "RSPickBufferMaterialPass.h"
#include "../Component.h"

namespace Razor
{
	void RSPickBufferMaterialPass::Render(RenderPipelineEntityProperties& Properties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<Mesh>();
		for (auto RenderingEntity : View)
		{
			Material& EntityMat = CurrentScene->GetComponent<Mesh>(RenderingEntity).Model->GetMaterial();
			auto View = CurrentScene->GetEntitiesWithComponents<Material>();

			if (Properties.Properties.find(RenderingEntity) == Properties.Properties.end())
			{
				continue;
			}
			EntityRenderProperty& Property = Properties.Properties[RenderingEntity];
			Property.GeneratePropertySlots(EntityMat.Materials.size());
			for (int i = 0; i < EntityMat.Materials.size(); i++)
			{
				PropertySlot& Slot = Property.GetPropertySlot(i);
				//Red, Green, Blue
				std::uint32_t Id = (std::uint32_t)RenderingEntity;
				glm::vec3 EntityColor = glm::vec3((Id & 0x000000FF) / 255.0f,
					((Id & 0x0000FF00) >> 8) / 255.0f,
					((Id & 0x00FF0000) >> 16) / 255.0f);
				Slot.AddProperty<glm::vec3>("entitycolor", EntityColor);
			}
		}
	}
}