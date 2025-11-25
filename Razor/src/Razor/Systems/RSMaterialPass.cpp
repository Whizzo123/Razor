#include "RSMaterialPass.h"
#include "../Component.h"
#include "../Engine.h"

namespace Razor
{

	void RSMaterialPass::Render(RenderPipelineEntityProperties& Properties)
	{
		auto View = CurrentScene->GetEntitiesWithComponents<Mesh>();
		for (auto RenderingEntity : View)
		{
			Mesh& EntityMesh = CurrentScene->GetComponent<Mesh>(RenderingEntity);
			AssetWrapper<Model>* Model = Engine::Get().GetAssetDirectory()->ProcessRequest<Razor::Model>(EntityMesh.mKey);
			Material& EntityMat = Model->asset.GetMaterial();
			if (Properties.Properties.find(RenderingEntity) == Properties.Properties.end())
			{
				continue;
			}
			EntityRenderProperty& Property = Properties.Properties[RenderingEntity];
			Property.GeneratePropertySlots(EntityMat.Materials.size());
			for (int i = 0; i < EntityMat.Materials.size(); i++)
			{
				PropertySlot& Slot = Property.GetPropertySlot(i);
				Slot.AddProperty<int>("material.texture_diffuse", 0);
				Slot.AddProperty<int>("material.texture_normal", 0);
				Slot.AddProperty<int>("material.texture_specular", 0);
				Slot.AddProperty<int>("material.texture_height", 0);
				Slot.AddProperty<glm::vec3>("material.diffuse", EntityMat.Materials[i].Diffuse);
				Slot.AddProperty<glm::vec3>("material.ambient", EntityMat.Materials[i].Ambient);
				Slot.AddProperty<glm::vec3>("material.specular", EntityMat.Materials[i].Specular);
				Slot.AddProperty<float>("material.shininess", EntityMat.Materials[i].Shininess);
				Slot.AddProperty<int>("material.bUseTextures", 0);
			}
		}
	}

}