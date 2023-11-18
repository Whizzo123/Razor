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

			// TODO get mesh index through here also add textures to material component
			Property.AddIntProperty("material.texture_diffuse", 0);
			Property.AddIntProperty("material.texture_normal", 0);
			Property.AddIntProperty("material.texture_specular", 0);
			Property.AddIntProperty("material.texture_height", 0);
			Property.AddVec3Property("material.diffuse", EntityMat.Materials[0].Diffuse);
			Property.AddVec3Property("material.ambient", EntityMat.Materials[0].Ambient);
			Property.AddVec3Property("material.specular", EntityMat.Materials[0].Specular);
			Property.AddVec3Property("material.shininess", EntityMat.Materials[0].Shininess);
			Property.AddBoolProperty("material.bUseTextures", 0);
		}
	}

}