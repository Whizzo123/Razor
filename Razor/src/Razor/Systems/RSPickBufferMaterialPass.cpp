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
				glm::vec3 EntityColor = glm::vec3(((1 >> 16) & 0xff) / 255.0f,
					((1 >> 8) & 0xff) / 255.0f, 
					(1 & 0xff) / 255.0f);
				Slot.AddProperty<glm::vec3>("entitycolor", EntityColor);
			}
		}
	}
}