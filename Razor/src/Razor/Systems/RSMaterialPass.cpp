#include "RSMaterialPass.h"
#include "../Component.h"

namespace Razor
{

	void RSMaterialPass::Render(std::array<RenderPipelineEntityProperties, MAX_ENTITIES>& Properties)
	{
		for (Entity RenderingEntity : Entities)
		{
			Material& EntityMat = Coordinator->GetComponent<Material>(RenderingEntity);
			
			RenderPipelineEntityProperties Property = Properties[RenderingEntity];
			Property.GeneratePropertySlots(EntityMat.Materials.size());
			// TODO get mesh index through here also add textures to material component
			for (int i = 0; i < EntityMat.Materials.size(); i++)
			{
				RenderPipelineEntityProperties::PropertySlot Slot = Property.GetPropertySlot(i);
				Slot.AddIntProperty("material.texture_diffuse", 0);
				Slot.AddIntProperty("material.texture_normal", 0);
				Slot.AddIntProperty("material.texture_specular", 0);
				Slot.AddIntProperty("material.texture_height", 0);
				Slot.AddVec3Property("material.diffuse", EntityMat.Materials[0].Diffuse);
				Slot.AddVec3Property("material.ambient", EntityMat.Materials[0].Ambient);
				Slot.AddVec3Property("material.specular", EntityMat.Materials[0].Specular);
				Slot.AddFloatProperty("material.shininess", EntityMat.Materials[0].Shininess);
				Slot.AddBoolProperty("material.bUseTextures", 0);
			}
		}
	}

}