#include "RSMaterialPass.h"
#include "../Component.h"

namespace Razor
{

	void RSMaterialPass::Render(RenderPipelineEntityProperties& Properties)
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
				Slot.AddIntProperty("material.texture_diffuse", 0);
				Slot.AddIntProperty("material.texture_normal", 0);
				Slot.AddIntProperty("material.texture_specular", 0);
				Slot.AddIntProperty("material.texture_height", 0);
				Slot.AddVec3Property("material.diffuse", EntityMat.Materials[i].Diffuse);
				Slot.AddVec3Property("material.ambient", EntityMat.Materials[i].Ambient);
				Slot.AddVec3Property("material.specular", EntityMat.Materials[i].Specular);
				Slot.AddFloatProperty("material.shininess", EntityMat.Materials[i].Shininess);
				Slot.AddBoolProperty("material.bUseTextures", 0);
			}
		}
	}

}